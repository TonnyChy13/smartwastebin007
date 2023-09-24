<?php

$host = "localhost";
$dbname = "id21221155_smartwastebin007";              // Database name
$username = "id21221155_smartbin";		// Database username
$password = "Smartbin007.";	        // Database password

// Establish connection to MySQL database
$conn = new mysqli($host, $username, $password, $dbname);

// Check if connection established successfully
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} else {
    echo "Connected to MySQL database. ";
}

// Get date and time variables
date_default_timezone_set('Asia/Dhaka');
$d = date("Y-m-d");
$t = date("H:i:s");

echo "Date and Time is ";
echo date("Y-m-d");

// If values sent by NodeMCU are not empty, insert into MySQL database table
// if (!empty($_POST['sendval']) && !empty($_POST['sendval2']) && !empty($_POST['sendval3'])) {
if ($_SERVER["REQUEST_METHOD"] == "POST"){
    $temp = $_POST['sendval'];
    $hum = $_POST['sendval2'];
    $level = $_POST['sendval3'];
    echo "Inserting Values in Tables";
    // Update your table name here
    $sql = "INSERT INTO SmartbinData (date, time, temp, hum, level) VALUES ('".$d."', '".$t."', '".$temp."','".$hum."','".$level."')";

    if ($conn->query($sql) === TRUE) {
        echo "Values inserted in MySQL database table.";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}

// Close MySQL connection
$conn->close();

?>
