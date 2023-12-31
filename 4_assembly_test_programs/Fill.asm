// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed.
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
(BEGIN)
  @8192
  D=A
  @R0
  M=D


  @i
  M=0

  @SCREEN
  D=A
  @address
  M=D

  @color
  M = 0

  @KBD
  D=M
  @LOOP
  D;JEQ

  @color
  M = -1


(LOOP)
  @i
  D=M
  @R0
  D=D-M
  @BEGIN
  D;JEQ

  @color
  D=M
  @address
  A=M
  M=D

  @i
  M=M+1
  @1
  D=A
  @address
  M=D+M
  @LOOP
  0;JMP
