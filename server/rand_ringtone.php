<?php

$files = glob('ringtones/*');

$n = rand(0, count($files) - 1);

$file = $files[$n];

$scheme = $_SERVER['REQUEST_SCHEME'];
$host = $_SERVER['HTTP_HOST'];
$path = dirname($_SERVER['REQUEST_URI']);

echo "$scheme://{$host}{$path}/$file";
