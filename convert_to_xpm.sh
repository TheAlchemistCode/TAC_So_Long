#!/bin/sh
# Converts all PNG files in 'assets pack 1' to XPM format in the 'assets' directory

mkdir -p assets
for file in "assets pack 1"/*.png; do
    filename=$(basename "$file" .png)
    echo "Converting $file to assets/${filename}.xpm"
    convert "$file" "assets/${filename}.xpm"
done
echo "Conversion complete!"