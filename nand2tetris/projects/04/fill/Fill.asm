// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.
// screen = screen + r.32 + c/16;
// Put your code here.

@SCREEN
D=A
@R0
M=D
@8191
D=D+A
@R2
M=D

(LOOP)
@SCREEN
D=A
@R0
M=D
D=0
@KBD
D=D-M

@SCREENON
D;JGT
@SCREENOFF
0;JMP

(SCREENON)
@R0
A=M
D=A
M=1
@R2
D=D-M
@LOOP
D;JGE
@R0
M=M+1
@SCREENON
0;JMP


(SCREENOFF)
@R0
A=M
D=A
M=0
@R2
D=D-M
@LOOP
D;JGE
@R0
M=M+1
@SCREENOFF
0;JMP





