#!/bin/bash
read foldername
read filename
~/nand2tetris/nand2tetris/projects/07/VMTranslator.o ~/nand2tetris/nand2tetris/projects/07/$foldername/$filename $foldername/$filename.asm
~/nand2tetris/nand2tetris/tools/CPUEmulator.sh ~/nand2tetris/nand2tetris/projects/07/$foldername/$filename.tst

#a file to make work check easy
