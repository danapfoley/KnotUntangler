# KnotUntangler
Untangles knots through Gauss Code - made for my senior I.S.

To operate:

1) Ensure that the file KnotList.txt can properly be read by the program. If the file path needs to be changed in the code, this can be done by altering KnotListFilePath near the beginning of main.cpp. This text file provides some example knots that each stress the program to its limits in their own ways. Feel free to add your own knots to this list in Extended Gauss Code to have them appear in the list of choices.
2) In main.cpp and Knot.h, respectively, you may set the definitions of Debug and knotDebug to either 1 or 0. 1 is chosen by default for each, which provides detailed output of how each function is operating during the untangling process. Choosing 0 for both will result in a very simple output - just the beginning and ending knots, before and after untangling.
3) Run the program and select a knot to untangle in the menu, or input your own knot for a single-time use. Observe the untangling output generated. If you entered your own knot in either the file or the console, be aware that the program cannot perform checks to ensure the accuracy of the Extended Gauss Code. Expect errors if a knot is encoded incorrectly.
