<?php
$host = "localhost";
$dbname = "id21221155_smartwastebin007";
$username = "id21221155_smartbin";
$password = "Smartbin007.";

$conn = new mysqli($host, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT Date, Time, Temperature, Humidity, Waste Level FROM Bin_2_data ORDER BY date DESC, time DESC";
$result = $conn->query($sql);

$data = array(); // Create an empty array to store the fetched data

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $data[] = $row; // Add each row to the data array
    }
}

$conn->close();

// Set cache control headers to prevent caching
header("Cache-Control: no-cache, must-revalidate"); // HTTP 1.1
header("Pragma: no-cache"); // HTTP 1.0
header("Expires: Sat, 01 Jan 2000 00:00:00 GMT"); // Date in the past

header('Content-Type: application/json'); // Set the response header to JSON
echo json_encode($data); // Convert the data array to JSON and echo the response
?>
