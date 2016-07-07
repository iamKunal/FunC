<?php
$name = $_GET['name'];
$email = $_GET['email'];
$feed = $_GET['feed'];
$feed = urldecode($feed);

date_default_timezone_set('Europe/London');
$time = $_SERVER['REQUEST_TIME'];
$time = date("d/m/Y h:i:s a", time());
$write = "Name :  " . $name . "\nEmail : " . $email . "\nAt " . $time . " GMT\n\nMessage  : \t" . $feed . "\n\n---x----\n\n\n";


$old = file_get_contents("feed.txt");
$fl= fopen("feed.txt", "w");
$write = $write . $old;
fwrite($fl, $write);
fclose($fl);
?>
Thanks ! Your request has been submitted ! You can view your request <a href="feed.txt" target="_blank">here</a>.