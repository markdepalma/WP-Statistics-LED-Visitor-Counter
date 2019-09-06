<?php

$servername = "localhost";
$username = "markblogwp";
$password = "password";
$dbname = "markdepalmablogwp";

$conn = new mysqli($servername, $username, $password, $dbname);

$sql = "SELECT count(*) AS count FROM markdepalmablogwp.wp_statistics_visitor 
WHERE last_counter = DATE_FORMAT(NOW(), '%Y-%m-%d')";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
	echo $row["count"];
    }
}

$conn->close();

?>
