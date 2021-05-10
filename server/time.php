<?php

$utime = sprintf('%.4f', microtime(TRUE));
$time = DateTime::createFromFormat('U.u', $utime);

$tz = date_default_timezone_get();
$time->setTimeZone(new DateTimeZone($tz));

echo $time->format('Y-m-d H:i:s.u');
