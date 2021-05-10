<?php
//die("");

function ring_this_day($now, $day_offset) {
    $ring_at = "07:55";
    $days = [1, 2, 3, 4, 5];

    $expr = "+$day_offset day $ring_at";
    $ts = strtotime($expr, $now);

    // in the past?
    if ($ts < $now) {
	return false;
    }

    // "allowed" day?
    $day = date('N', $ts);
    if (array_search($day, $days) === false) {
	return false;
    }

    return $ts;
}

$now = time();

// never look more than 10 days away
for ($i = 0 ; $i < 10 ; ++$i) {
    $ts = ring_this_day($now, $i);
    if ($ts !== false) {
	$str = date("Y-m-d H:i:s", $ts);
	die($str);
    }
}
