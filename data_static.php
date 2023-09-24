<!DOCTYPE html>
<html>
<head>
    <title>Smart Bin Data</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
            background-color: #f2f2f2;
        }

        h1 {
            text-align: center;
        }

        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }

        th, td {
            padding: 10px;
            text-align: left;
            border-bottom: 1px solid #ccc;
        }

        th {
            background-color: #f9f9f9;
        }
    </style>
</head>
<body>
    <h1>Smart Bin Data</h1>

    <table>
        <tr>
            <th>Date</th>
            <th>Time</th>
            <th>Temperature</th>
            <th>Humidity</th>
            <th>Level</th>
        </tr>

        <?php
        $host = "localhost";        // host = localhost because the database is hosted on the same server where PHP files are hosted
        $dbname = "id21221155_smartwastebin007";  // Database name
        $username = "id21221155_smartbin";    // Database username
        $password = "Smartbin007.";    // Database password

        // Establish connection to MySQL database
        $conn = new mysqli($host, $username, $password, $dbname);

        // Check if connection established successfully
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        // Select values from MySQL database table
        $sql = "SELECT date, time, temp, hum, level FROM SmartbinData ORDER BY date DESC,time DESC";
        $result = $conn->query($sql);

        if ($result->num_rows > 0) {
            // output data of each row
            while($row = $result->fetch_assoc()) {
                echo "<tr>";
                echo "<td>" . $row["date"] . "</td>";
                echo "<td>" . $row["time"] . "</td>";
                echo "<td>" . $row["temp"] . "°C</td>";
                echo "<td>" . $row["hum"]  . "%</td>";
                echo "<td>" . $row["level"]. "%</td>";
                echo "</tr>";
            }
        } else {
            echo "<tr><td colspan='5'>No results</td></tr>";
        }

        $conn->close();
        ?>
    </table>
</body>
</html>
