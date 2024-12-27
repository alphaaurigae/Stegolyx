#!/bin/bash


input_dir="input"
output_dir="output"
workdir="bin"
bin="stegolyx"

lsb_dir="lsb"


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
identify -verbose $input_dir/$input_main

#rm -f $workdir/$lsb_dir/$output_image_textsecret
mkdir -p "$output_dir/$lsb_dir"
echo "Embedding text into image..."
echo ""
echo "$workdir/$bin -mode text -method lsb -i $input_dir/$input_main -t "$input_text" -o $output_dir/$lsb_dir/$output_image_textsecret"
echo ""
$workdir/$bin -mode text -method lsb -i $input_dir/$input_main -t "$input_text" -o $output_dir/$lsb_dir/$output_image_textsecret
echo ""
echo ""
echo "Extracting text from image..."
echo ""
echo "$workdir/$bin -mode text -method lsb -i $output_dir/$lsb_dir/$output_image_textsecret -o $output_dir/$lsb_dir/$decode_text_textfile -d "
echo ""
$workdir/$bin -mode text -method lsb -i $output_dir/$lsb_dir/$output_image_textsecret -o $output_dir/$lsb_dir/$decode_text_textfile -d
echo ""
echo ""


#rm -f $workdir/$lsb_dir/$output_image_textsecret
#mkdir -p "$output_dir/$lsb_dir"
#echo "Embedding text into image..."
#echo ""
#echo "$workdir/$bin -mode text -method lsb -i $input_dir/$input_main -t "$input_text" -o $output_dir/$lsb_dir/$output_image_textsecret"
#echo ""
#$workdir/$bin -mode text -method lsb -i $input_dir/$input_main -t "$input_text" -o $output_dir/$lsb_dir/$output_image_textsecret
#echo ""
#echo ""
#echo "Extracting text from image..."
#echo ""
#echo "$workdir/$bin -mode text -method lsb -i $output_dir/$lsb_dir/$output_image_textsecret -o $output_dir/$lsb_dir/$decode_text_textfile -d "
#echo ""
#$workdir/$bin -mode text -method lsb -i $output_dir/$lsb_dir/$output_image_textsecret -o $output_dir/$lsb_dir/$decode_text_textfile -d
#echo ""
#echo ""


#echo "identify input foreground image to be hidden"
##identify -verbose $input_dir/$input_fg
#echo "Embedding image into image..."
#echo ""
#echo "$workdir/$bin -mode image -method -i $input_dir/$input_main -fg $input_dir/$input_fg -o $output_dir/$lsb_dir/$output_image_imagesecret"
#$workdir/$bin -mode image -method lsb -i $input_dir/$input_main -fg $input_dir/$input_fg -o $output_dir/$lsb_dir/$output_image_imagesecret
#echo ""
#echo ""
#echo "Extracting hidden image..."
#echo ""
#echo "$workdir/$bin -mode image -method lsb -i $output_dir/$lsb_dir/$output_image_imagesecret -o $output_dir/$lsb_dir/$decode_image_imagesecret -d "
#echo ""
#$workdir/$bin -mode image -method lsb -i $output_dir/$lsb_dir/$output_image_imagesecret -o $output_dir/$lsb_dir/$decode_image_imagesecret -d


