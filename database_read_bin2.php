<?php



    $host = "localhost";		         // host = localhost because database hosted on the same server where PHP files are hosted
    $dbname = "id21221155_smartwastebin007";              // Database name
    $username = "id21221155_smartbin";		// Database username
    $password = "Smartbin007.";	        // Database password


// Establish connection to MySQL database
$conn = new mysqli($host, $username, $password, $dbname);


// Check if connection established successfully
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

else { echo "Connected to mysql database. <br>"; }


// Select values from MySQL database table
$sql = "SELECT Date, Time, Temperature, Humidity, Waste Level FROM Bin_2_data ORDER BY time DESC"; // Add ORDER BY clause

$result = $conn->query($sql);

echo "<center>";

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<strong>Date:</strong> " . $row["Date"]. " &nbsp <strong>Time:</strong> " . $row["Time"]. " &nbsp <strong> Temperature:</strong> " . $row["Temperature"]." &nbsp <strong>Humidity:</strong>" .$row["Humidity"]." &nbsp <strong>Waste Level:</strong>" .$row["Waste Level"]. "<p>";
    }
} else {
    echo "0 results";
}

echo "</center>";

$conn->close();

?>
