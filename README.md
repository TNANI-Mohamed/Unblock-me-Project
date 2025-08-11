The project contains 2 main Classes : 
-Game : that sets up the Board Game.
-State : that keeps track of the States while playing the game. (See the Report for more INFO).


ExRushHour : contains the Data for experimental results.
Results : contains certain outputs used in the Report.

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

HOW TO RUN THE CODE IN main.cpp:

There are two ways of running the code :

the program will ask you to input 0 or 1 : 
 
0 runs the program on a certain board you input.
1 runs the program on certain cases from the Database provided in Question 6.

**************************************************************************************

IF YOU CHOSE 0 :

-Input the board game data in the same format given in the project file.

-Choose if you want to use heuristics by typing (1 : if you want , 0 : if not)

--------------------------------------------------------------------
YOU CHOSE 0 : the program will give you the shortest_path solution
and the time of execution.

---------------------------------------------------------------------
YOU CHOSE 1 : the program will ask you to choose heuristics you want to Apply  

There are 3 types of Heuristics : "trivial", "blocking_cars", "distance_to_exit"
For each Heuristic type 1, if you want to apply it, 0 if not.

EXAMPLE : if you want to apply "trivial" and "distance_to_exit" : TYPE "101" .

The program will give you thesolution and the time of execution for each Heuristic.


*************************************************************************************

YOU CHOSE 1 :

The program will ask you the range of the cases you want to test from the Database.
Type : "starting_case > final_case"

EXAMPLE : if you want to test the code on Games 4 to 18 : TYPE "4 > 18" .

Then the program will ask you the same Questions as the Choice 0 (READ ABOVE FOR THE EXPLANATION)


****************************************************************************************
