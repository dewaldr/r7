#!/usr/bin/php
<?php
    if ($argc == 2 ) {
        $raw = $argv[1];
        $hashed = hash( 'sha256', $raw );
        echo $hashed, "\n";
    }
    else {
        echo "usage r7passwd passwd\n";
    }
?>
