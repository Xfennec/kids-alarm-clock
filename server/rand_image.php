<?php

$files = glob('images/*');

$n = rand(0, count($files) - 1);

$file = $files[$n];
$content = file_get_contents($file);

header('Content-Type: image/jpeg');
header('Content-Length: ' . filesize($file));
echo $content;
