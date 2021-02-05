/* Author: Mustafa Yagcioglu
 * Author email: yagciogluengieering@gmail.com
 * Coding date:  jan 2021
 */
#include <iostream>
#include <cstdlib>
#include <mutex>
#include <map>
#include <set>
#include <assert.h>

 /**
 * Finite State Machine implementation class
 *
 * @State: generic type for the states
 * @Input: generic type for the inputs
 */
template<typename State, typename Input >
class fsm
{
	// A typedef to make for easier reading
	typedef void (*FuncPtr)();

	typedef std::pair<State,Input> StateInputPair;
	typedef std::pair<State,FuncPtr> LastStateCallbackPair;
	typedef std::map<StateInputPair,LastStateCallbackPair> StateDiagram;
	/*
	 * C++ standard template library "map" is used for the solution. Key member of
	 * "StateDiagram" map is "StateInputPair" which contains "current state" and
	 * "Input" pair.
	 * By the help of this structure, we can determine the required action (required
	 * action is simply nextstate-Function pair in our case). This pair is stored in
	 * "LastStateCallbackPair" pair structure.
   *
	 * By the help of this dynamic structure, it is possible to define new transitions
	 * (adding new transition means adding a new element to "StateDiagram" instance in
	 * our case)
   */

	// StateDiagram instance
	StateDiagram sd;

	/*
	 * Keep list of states, if the state is not in this list, the state cannot be
	 * used in the transitions
	 */
	std::set<State> listOfStates;

	// current state
	State currentState;

	/*
	 * pointer to the fallback function. This function will be call in all
	 * invalid transition requests
	 */
	FuncPtr fallBackFunction;

	// added to make public functions thread safe
	std::mutex mutex_;

public:

	/*
	 *  Constructor
	 */
	fsm(){}

	/*
	 *  Add state to state table
	 *
	 *  @param s: state to add
	 */
	void AddState(State s){
		mutex_.lock();
		if(listOfStates.find(s) == listOfStates.end() )
		listOfStates.insert(s);
		mutex_.unlock();
	}

	/*
	 *  Set fallback function
	 *
   *  @param fb: Fallback Function
	 */
	void SetFallback(FuncPtr fb){
		mutex_.lock();
		fallBackFunction = fb;
		mutex_.unlock();
	}

	/*
	 *  Add transition to statediagram map
	 *
	 *  @param s1: state before transition
	 *  @param s2: state after transition
	 *  @param inp: input that trig transition
   *  @param cb: Callback Function
	 */
	void AddTransition(State s1, State s2 , Input inp , FuncPtr cb){
		mutex_.lock();

		if(listOfStates.find(s1) == listOfStates.end())
		{
			std::cout << " FAILURE, state " << s1 << " is not in the state list " << std::endl;
			fallBackFunction();

		}else if (listOfStates.find(s2) == listOfStates.end())
		{
			std::cout << " FAILURE, state " << s2 << " is not in the state list " << std::endl;
			fallBackFunction();

		}else{
			StateInputPair _tmp_si = std::make_pair(s1,inp);
			LastStateCallbackPair _tmp_sc = std::make_pair(s2,cb);

			sd.insert(std::make_pair(_tmp_si,_tmp_sc));
		}
		mutex_.unlock();
	}

	/*
	 * Set current state to a defined state directly
	 *
	 *  @param s: state
	 */
	void ResetMachine(State s){
		mutex_.lock();
		if(listOfStates.find(s) == listOfStates.end())
		{
			std::cout << " FAILURE, state " << s << " is not in the state list " << std::endl;
			fallBackFunction();

		}else{
			currentState = s;
		}
		mutex_.unlock();
	}

	/*
	 * Get current State
	 *
	 *  @return Current State
	 */
	State GetState(){
		return currentState;
	}

	/*
	 * If there is a valid transition for "current state - input" pair, call
	 * callback function and change current state
	 *
	 *  @param inp: input
	 */
	void Step(Input inp){
		mutex_.lock();
		StateInputPair _pair = std::make_pair(currentState,inp);

		auto it=sd.find(_pair);
		if (it==sd.end()){
			fallBackFunction();
		}else{
			currentState = it->second.first;
			it->second.second();
		}
		mutex_.unlock();
	}

	/*
	 * Print current State
	 */
	void printCurrentState(){
		std::cout << " current State:  "  << currentState << std::endl;
	}
};



/***************************************************************************
Global variables and functions for testcases
*/

//example class, used in test cases
/*
 * Print current State
 */
class StateClass {
	std::string name;
};

/*
 * User defined callback function, is used in testcases
 */
void callback1(){
	std::cout << "callback1 is called" << std::endl;
}

/*
 * User defined callback function, is used in testcases
 */
void callback2(){
	std::cout << "callback2 is called" << std::endl;
}

/*
 * User defined fallback function, is used in testcases
 */
void fallback(){
	std::cout << "fallback is called" << std::endl;
}

/*
 *  This function is defined for testcases
 *
 *  Checks first two parameters equality and print accordingly
 *
 *  This function works for only C++ defined types and user defined class object
 *  pointers.
 *
 *  @param left: left value for comparision
 *  @param right: right value for comparision
 *  @param successMessage: Message to give if left and right are equal
 *  @param failureMessage: Message to give if left and right are NOT equal
 *  @param line: Line, that this message is called
 *  @param fileName: File name, that this message is called
 */
template<typename T>
void myAssert(T left, T right, std::string successMessage, std::string failureMessage, int line, std::string fileName )
{
	if (left == right){
		std::cout << successMessage << std::endl;
	}else{
		std::cout << failureMessage << std::endl;
		std::cout << "fileName: " << fileName << std::endl;
		std::cout << "line: " << line << std::endl;
	}

}

/*
 * State transitions are tested with
 * states: string
 * inputs: char
 */
void testCase1(){

	std::cout << " Test Case 1 starts  "  << std::endl;

	fsm<std::string,char> fsmInstance;

	//Define states type as "string"
	std::string s1 = "s1";
	std::string s2 = "s2";
	std::string s3 = "s3";

	//Define input type as "char"
	char inp1 = 'i';
	char inp2 = 'j';
	char inp3 = 'k';

	//Set fallback function for invalid transactions
	fsmInstance.SetFallback(fallback);

	//Add states
	fsmInstance.AddState(s1);
	fsmInstance.AddState(s2);
	fsmInstance.AddState(s3);


	//Add transitions
	fsmInstance.AddTransition( s1, s2 , inp1 , callback1);
	fsmInstance.AddTransition( s2, s3 , inp2 , callback2);

	//Set initial state
	fsmInstance.ResetMachine(s1);

	myAssert(fsmInstance.GetState(),s1, " TEST_SUITE_COMMAND: SUCCESS","FAILURE message",__LINE__, __FILE__);

	fsmInstance.Step(inp1);
	myAssert(fsmInstance.GetState(),s2, " TEST_SUITE_COMMAND: SUCCESS, callback 1 function should be called, check manually","FAILURE message, current state is not correct",__LINE__, __FILE__);

	fsmInstance.Step(inp2);
	myAssert(fsmInstance.GetState(),s3, " TEST_SUITE_COMMAND: SUCCESS, callback 2 function should be called, check manually","FAILURE message, current state is not correct",__LINE__, __FILE__);

	fsmInstance.Step(inp2);//invalid state transition
	myAssert(fsmInstance.GetState(),s3, " TEST_SUITE_COMMAND: SUCCESS, fallback function should be called, check manually","FAILURE message, current state is not correct",__LINE__, __FILE__);

	fsmInstance.ResetMachine(s2);//set state to s2 directly
	myAssert(fsmInstance.GetState(),s2, " TEST_SUITE_COMMAND: SUCCESS", "FAILURE message, current state is not correct",__LINE__, __FILE__);

	std::cout << " Test Case 1 ends  "  << std::endl;

}

/*
 * State transitions are tested with
 * states: user defined StateClass*
 * inputs: integer
 */
void testCase2(){

	std::cout << std::endl<< " Test Case 2 starts  "<< std::endl  << std::endl;

	fsm<StateClass*,int> fsmInstance;

	//Define states type as State class
	StateClass* s1;
	StateClass* s2;
	StateClass* s3;

	//Define input type as "int"
	int inp1 = 4;
	int inp2 = 5;
	int inp3 = 6;

	//Set fallback function for invalid transactions
	fsmInstance.SetFallback(fallback);

	//Add states
	fsmInstance.AddState(s1);
	fsmInstance.AddState(s2);
	fsmInstance.AddState(s3);

	//Add transitions
	fsmInstance.AddTransition( s1, s2 , inp1 , callback1);
	fsmInstance.AddTransition( s2, s3 , inp2 , callback2);

	//Set initial state
	fsmInstance.ResetMachine(s1);

	myAssert(fsmInstance.GetState(),s1, "SUCCESS","FAILURE message",__LINE__, __FILE__);

	fsmInstance.Step(inp1);
	myAssert(fsmInstance.GetState(),s2, "SUCCESS, callback 1 function must be called, check manually","FAILURE message, current state is not correct",__LINE__, __FILE__);

	fsmInstance.Step(inp2);
	myAssert(fsmInstance.GetState(),s3, "SUCCESS, callback 2 function must be called, check manually","FAILURE message, current state is not correct",__LINE__, __FILE__);

	fsmInstance.Step(inp2);//invalid state transition
	myAssert(fsmInstance.GetState(),s3, "SUCCESS, fallback function must be called, check manually","FAILURE message, current state is not correct",__LINE__, __FILE__);

	fsmInstance.ResetMachine(s2);//set state to s2 directly
	myAssert(fsmInstance.GetState(),s2, " TEST_SUITE_COMMAND: SUCCESS", "FAILURE message, current state is not correct",__LINE__, __FILE__);

	std::cout << std::endl<< " Test Case 2 ends  " << std::endl << std::endl;

}

/*
 * State transitions are tested with
 * states: string
 * inputs: char
 * In this testcase, invalid input cases are tested
 */
void testCase3(){

	std::cout << std::endl<< " Test Case 3 starts  " << std::endl << std::endl;

	fsm<std::string,char> fsmInstance;

	//Define states type as "string"
	std::string s1 = "s1";
	std::string s2 = "s2";
	std::string s3 = "s3";

	//Define input type as "char"
	char inp1 = 'i';
	char inp2 = 'j';
	char inp3 = 'k';

	//Set fallback function for invalid transactions
	fsmInstance.SetFallback(fallback);

	//Add states
	fsmInstance.AddState(s1);
	fsmInstance.AddState(s2);

	//Add transitions
	fsmInstance.AddTransition( s1, s2 , inp1 , callback1);

	fsmInstance.AddTransition( s2, s3 , inp2 , callback2);
	std::cout << " TEST_SUITE_COMMAND: s3 is not a defined state, sytem should give an error, fallback function should be called,  check manually  "  << std::endl;

	//Set initial state
	fsmInstance.ResetMachine(s1);

	//check if state is set to s1
	myAssert(fsmInstance.GetState(),s1, "SUCCESS","FAILURE message",__LINE__, __FILE__);

	fsmInstance.ResetMachine(s3);
	std::cout << " TEST_SUITE_COMMAND: s3 is not a defined state, sytem should give an error, fallback function should be called,  check manually  "  << std::endl;

	fsmInstance.Step(inp1);
	myAssert(fsmInstance.GetState(),s2, " TEST_SUITE_COMMAND: SUCCESS, callback 1 function should be called, check manually","FAILURE message, current state is not correct",__LINE__, __FILE__);

	fsmInstance.Step(inp2);//invalid state transition
	myAssert(fsmInstance.GetState(),s2, " TEST_SUITE_COMMAND: SUCCESS, fallback function should be called, check manually","FAILURE message, current state is not correct",__LINE__, __FILE__);

	std::cout << std::endl<< " Test Case 3 ends  "  << std::endl << std::endl;

}

/*
 * main method to call testcases
 */
int main()
{

	testCase1();
	testCase2();
	testCase3();

	return 0;
}
