#!/bin/bash

# Specify the filename and the text to add
filename="example.txt"
text="Hello! This is my CS377 Final Project."

# Create the file and append the text
echo "$text" > "$filename"

# Display success message
echo "Text file created: $filename"