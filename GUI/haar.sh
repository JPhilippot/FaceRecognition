cp $1 ./img/toTreat.jpg
./haar ./img/toTreat.jpg
convert ./img/cropped.jpg -resize 442x442 ./img/haar.pgm
rm ./img/toTreat.jpg