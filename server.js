const express = require("express");
const bodyParser = require("body-parser");
const sqlite3 = require("sqlite3").verbose();
const path = require("path");

const app = express();
const port = 3000;

// Middleware to parse form data
app.use(bodyParser.urlencoded({ extended: true })); // For parsing form data (x-www-form-urlencoded)
app.use(bodyParser.json());

// Serve the HTML page
app.use(express.static(path.join(__dirname, "public")));
// Fallback route to serve index.html for undefined routes
app.get("*", (req, res) => {
  res.sendFile(path.join(__dirname, "public"));
});

// Set up SQLite database
let db = new sqlite3.Database("./complaints.db", (err) => {
  if (err) {
    console.error("Could not open database:", err.message);
  } else {
    console.log("Connected to the SQLite database.");
  }
});

// Create complaints table if it doesn't exist
db.run(`
  CREATE TABLE IF NOT EXISTS complaints (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    email TEXT NOT NULL,
    complaint TEXT NOT NULL,
    status TEXT DEFAULT 'Open'
  );
`);

// Handle complaint form submission
app.post("/submit-complaint", (req, res) => {
  const { name, email, complaint } = req.body;

  if (!name || !email || !complaint) {
    return res.status(400).send("All fields are required.");
  }

  const sql = `INSERT INTO complaints (name, email, complaint) VALUES (?, ?, ?)`;
  const params = [name, email, complaint];

  db.run(sql, params, function (err) {
    if (err) {
      console.error("Error inserting complaint:", err.message);
      return res
        .status(500)
        .send("An error occurred while submitting your complaint.");
    }
    // Log the complaint details in the terminal
    console.log("Complaint Submitted Successfully:");
    console.log(`ID: ${this.lastID}`);
    console.log(`Name: ${name}`);
    console.log(`Email: ${email}`);
    console.log(`Complaint: ${complaint}`);

    // Redirect with success query parameter
    res.redirect("/?success=true");
  });
});

// Start the server
app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});
