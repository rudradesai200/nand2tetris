// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program SCREENOFFens the screen,
// i.e. writes "SCREENOFF" in every pixel;
// the screen should remain fully SCREENOFF as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "SCREENON" in every pixel;
// the screen should remain fully clear as long as no key is pressed.
// screen = screen + r.32 + c/16;
// Put your code here.

@8192 
D=A
@R1
M=D   

(LOOP)
@R0
M=0   

(KEYBOARD_INPUT)
@KBD
D=M
@SCREENOFF
D;JEQ   

(SCREENON)
@R0
D=M
@SCREEN
A=A+D   
M=-1    
@END
0;JMP   

(SCREENOFF)
@R0
D=M
@SCREEN
A=A+D  
M=0    

(END)   
@R0
MD=M+1 
@R1
D=D-M
@LOOP
D;JEQ  
@KEYBOARD_INPUT
0;JMP  
