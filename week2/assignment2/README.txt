# Assignment 2:

1. This is for assignment 2 for CS344 at Oregon State University.
    
*To get the compiling file run the following command on the terminal:*
     
    gcc --std=gnu99 -o movies_by_year movies_by_year.c

*To run the file just run this command:

    ./movies_by_year 

# Result: 

When you enter option 1, you will get a directory with the required files.
To enter option 2, please restart the program by exiting with option 2, then run again with ./movies_by_year
Same for option 3, otherwise you will get the wrong result because of a segmentation fault error.

The file is called movie_by_year instead of movies_by_year because when I iterate through the files in the working directory,
if there are any files with the PREFIX movies_ in front of it will take it into account for size of file measurement. I want to avoid 
that by making a slight change from movies_by_year --> movie_by_year. 

As expected from the test cases on the Sample Program Execution. 
You may have to restart the program, to do so run:
    ./movies_by_year

    


    



