#!/bin/bash

repository_dir="/home/mmmm/Desktop/proj/decode_encode/stegano/2.openvc"
input_dir="input"
output_dir="output"
workdir="bin"
bin="stegano"

dwt_dir="dwt"


input_main="jpegPIA24431.jpg" # 91.1 kib
#input_main="xubuntu-vivid.png" #  459.3 kib
#input_main="1010maverick.png" # 890.2

#input_fg="567px-Larry-nefarius-v2.svg.png" # 32.2 kib
#input_fg="Tux.svg.png" # 297.1 kib
input_text="daremightythings"

output_image_textsecret="output_textsecret.png"
decode_text_textfile="decoded_text.txt"
output_image_imagesecret="output_imagesecret.png"
decode_image_imagesecret="decoded_image.png"

echo "identify input background image"
identify -verbose $repository_dir/$input_dir/$input_main

#rm -f $repository_dir/$workdir/$dwt_dir/$output_image_textsecret
mkdir -p "$repository_dir/$output_dir/$dwt_dir"
echo "Embedding text into image..."
echo ""
echo "$workdir/$bin -mode text -method lsb -i $repository_dir/$input_dir/$input_main -t "$input_text" -o $repository_dir/$output_dir/$dwt_dir/$output_image_textsecret"
echo ""
$workdir/$bin -mode text -method lsb -i $repository_dir/$input_dir/$input_main -t "$input_text" -o $repository_dir/$output_dir/$dwt_dir/$output_image_textsecret
echo ""
echo ""
echo "Extracting text from image..."
echo ""
echo "$workdir/$bin -mode text -method lsb -i $repository_dir/$output_dir/$dwt_dir/$output_image_textsecret -o $repository_dir/$output_dir/$dwt_dir/$decode_text_textfile -d "
echo ""
$workdir/$bin -mode text -method lsb -i $repository_dir/$output_dir/$dwt_dir/$output_image_textsecret -o $repository_dir/$output_dir/$dwt_dir/$decode_text_textfile -d
echo ""
echo ""

#echo "identify input foreground image to be hidden"
##identify -verbose $repository_dir/$input_dir/$input_fg
#echo "Embedding image into image..."
#echo ""
#echo "$workdir/$bin -mode image -method -i $repository_dir/$input_dir/$input_main -fg $repository_dir/$input_dir/$input_fg -o $repository_dir/$output_dir/$dwt_dir/$output_image_imagesecret"
#$workdir/$bin -mode image -method lsb -i $repository_dir/$input_dir/$input_main -fg $repository_dir/$input_dir/$input_fg -o $repository_dir/$output_dir/$dwt_dir/$output_image_imagesecret
#echo ""
#echo ""
#echo "Extracting hidden image..."
#echo ""
#echo "$workdir/$bin -mode image -method lsb -i $repository_dir/$output_dir/$dwt_dir/$output_image_imagesecret -o $repository_dir/$output_dir/$dwt_dir/$decode_image_imagesecret -d "
#echo ""
#$workdir/$bin -mode image -method lsb -i $repository_dir/$output_dir/$dwt_dir/$output_image_imagesecret -o $repository_dir/$output_dir/$dwt_dir/$decode_image_imagesecret -d


