# Generic-State-Machine
Implementation of a generic state machine

Author: Mustafa Yagcioglu

Author email: yagciogluengieering@gmail.com

Coding date:  jan 2021

*******************************************************************************
Change Log

This is the first version

*******************************************************************************
General information

This code implements a finite state machine

*******************************************************************************
Problem Definition

Write a class in C++ that implements a generic state machine with the following properties:
• It should be possible to register new states, transitions and fallback transitions at runtime.
◦ Fallback transitions are transitions that are triggered when input does not match any currently
registered transitions.
• It should be possible to register callbacks for transitions.
• All public methods of class should be thread safe.
• The types for states and inputs should be generic.
Following is an example usage of the FSM class instance.

fsm.AddState('a');
fsm.AddState('b');
fsm.AddTransition('a', 'b', "input1", callback1);
fsm.ResetMachine('a');
fsm.Step("input1");
assert(fsm.GetState() == 'b');
fsm.AddState('c');
fsm.AddTransition('b', 'c', "input2", callback2);
fsm.Step("input2");
assert(fsm.GetState() == 'c');

*******************************************************************************
Solution Description

Solution is based on the following structure:

typedef std::pair<State,Input> StateInputPair;
typedef std::pair<State,FuncPtr> LastStateCallbackPair;
typedef std::map<StateInputPair,LastStateCallbackPair> StateDiagram;

C++ standard template library "map" is used for the solution. Key value of
 "StateDiagram" map is "StateInputPair" which contains "current state" and
 "Input" pair.
 By the help of this structure, we can determine the required action (required
 action is simply nextstate-Function pair in our case). This pair is stored in
 "LastStateCallbackPair" pair structure.

By the help of this dynamic structure, it is possible to define new transitions.
 (adding new transition means adding a new element to "StateDiagram" instance in
 our case)

*******************************************************************************
Experimental Results

Code is tested with C++ standard types and user defined pointers as the types
 for states and inputs.
*******************************************************************************
Files

fsm.cpp:
Includes class implementation and testcases. They are not seperated to make
 compile process easy for different computers.

*******************************************************************************
Install

The code is compiled and run on windows 10. DEV-C++ is used as the development
 environment. C++ version was 14.

Following command is used for compile (within Dev-C++):
C:\"Program Files (x86)"\Dev-Cpp\MinGW64\bin\g++.exe "C:\FiniteStateMachine\fsm.cpp" -o "C:\FiniteStateMachine\fsm.exe" -std=c++14 -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\include" -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"C:\Program Files (x86)\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\c++" -L"C:\Program Files (x86)\Dev-Cpp\MinGW64\lib" -L"C:\Program Files (x86)\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib" -static-libgcc


Since the paths are not generic, it will be easy to compile the file with your
 own compiler with the above settings. I keep whole code in one file to minimize
 dependency errors on different platforms. In ideal case, you can locate
 fsm class in a different file.

*******************************************************************************
BUGS and IMPROVEMENTS

There are no known bugs

Engineering tests are completed, detailed verification and validation should be
 done by independent people

*******************************************************************************
