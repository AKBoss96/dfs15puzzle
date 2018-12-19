	#include <iostream>
	#include <string>
	#include <vector>
	#include <map>
	#include <stack>
	#include <cstdlib>
	#include <chrono>

	using namespace std;
	using namespace std::chrono; //to calculate running time
	int expandCount = 0, blank_pos;

	const string goalState = "123456789ABCDEF0"; //goal configuration

	struct State{     //structure representing each state of the search tree
		string order; //configuration of tiles
		string moves; //sequence of moves to follow
	};

	bool checkGoalState(State a){ 				// checking for the goal configuration
		return (a.order.compare(goalState)==0);
	}

	string performMove(string order, int x, int y){ //represents a move from current state to next state
		string newState(order);
		string temp = newState.substr(x,1);
		newState[x] = newState[y];
		newState[y] = temp[0];
		return newState;
	}

	State DepthFirstSearch(State startState, int maxDepth = 100){  //function to carry out DFS
		stack<State> fringe;      //set of states to be expanded ; Note - DFS uses a stack for its implementation
		map<string,int> visited;  //states which have already been expanded
		fringe.push(startState);  //pushing start state into the stack
		
		
		while(fringe.size()>0){
			State curState = fringe.top(); //current state is the top of stack
			fringe.pop(); //popping the current state from stack
			visited[curState.order] = curState.moves.length();
			expandCount += 1;
			vector<State> nextStates;
			
			blank_pos = curState.order.find("0");   
			if(blank_pos > 3){ //condition for UP move
				State newState;
				newState.order = performMove(curState.order, blank_pos, blank_pos-4);
				newState.moves = curState.moves;
				newState.moves += 'U'; 
				nextStates.push_back(newState);
			}
			if (blank_pos < 12){ //condition for DOWN move
				State newState;
				newState.order = performMove(curState.order, blank_pos, blank_pos+4);
				newState.moves = curState.moves;
				newState.moves += 'D';
				nextStates.push_back(newState);
			}
			if (blank_pos%4 < 3){ //condition for RIGHT move
				State newState;
				newState.order = performMove(curState.order, blank_pos, blank_pos+1);
				newState.moves = curState.moves;
				newState.moves += 'R';
				nextStates.push_back(newState);		
			}
			if (blank_pos%4 > 0){ //condition for LEFT move
				State newState;
				newState.order = performMove(curState.order, blank_pos, blank_pos-1);
				newState.moves = curState.moves;
				newState.moves += 'L';
				nextStates.push_back(newState);		
			}

			for(int i = 0; i < nextStates.size(); i++){
				if(nextStates[i].moves.length() > maxDepth){  
					continue;
				}
				if (checkGoalState(nextStates[i])){ 
				    return nextStates[i];
				}
				if (visited.find(nextStates[i].order) != visited.end() 
						&& visited[nextStates[i].order] < nextStates[i].moves.length()){
					continue;
				}
				fringe.push(nextStates[i]);
				
			}
		}
		return startState;
	}

	State IterativeDepthFirstSearch(State startState){ //function to perform DFS iteratively until depth 100 is reached
		for(int i = 1; i < 100; i++){
		    State searchResult = DepthFirstSearch(startState, i);
			if(checkGoalState(searchResult)){
			    cout<<"Solution found at depth " << i << endl;  // depth 
				return searchResult;
			}
		}
		
		return startState;
	}

	bool checkSolvable(State startState) //to check if a given input has a solution
	{
		int inversions = 0; //to count the number of inversions
    	for (int i = 0; i < 15; i++) 
	    { 
	        for (int j = i + 1; j < 16; j++) 
	        { 
	        	//value of i < value of j, but value at position i is greater than that at position j
	            if (startState.order[j]!='0' && startState.order[i]!='0' && (startState.order[i] > startState.order[j])) 
	            	inversions++;		
	        } 
	    }
	    
	    int row = 4 - (blank_pos/4); //identifying the row in which the empty tile is present
	    if (row & 1) 	//using bitwise to find parity
	    	return !(inversions & 1); 
	    else
	        return inversions & 1; 
	}

	int main(int argc, char* argv[]){
		State startState;
		startState.order = "";
		startState.order = argv[1];  //getting initial configuration as command-line argument
		blank_pos = startState.order.find("0"); //finding where the blank tile is located
		
		if(!checkSolvable(startState)) 
		{
			cout << "Solution cannot be found" << endl; //unsolvable initial configuration
			return 1;
		}
		
		time_point<system_clock> startTime = system_clock::now(); //time before Iterative DFS starts
		State searchResult = IterativeDepthFirstSearch(startState); 
		time_point<system_clock> endTime = system_clock::now(); //time after Iterative DFS returns 
		std::chrono::duration<double> runTime = endTime - startTime; //duration in seconds
		
		cout << "Moves: " << searchResult.moves << endl; //final move order to reach the goal state
		cout << "Number of nodes expanded: " << expandCount << endl; //metric for expansion of search tree
		cout << "Time taken: " << runTime.count()*1000 << " milliseconds" << endl; //duration in milliseconds
		cout << "Memory used: " << expandCount*(16+searchResult.moves.length()) << " bytes" << endl; //memory usage
		
		return 0;
	}