/*
 * graph.h
 *
 *  Created on: 11 sept. 2015
 *      Author: tnguyen
 */

#ifndef GRAPH_H_
#define GRAPH_H_
#include  "marking.h"



namespace pnapi
{
class Place;
class Marking;
class Transition;

class Graph {
private:
		std::vector<Marking> Mset_; // set of reachable markings
		//Marking * init_; // initial marking
public:
	Graph();
	virtual ~Graph();
	Graph(const Marking & m);
	//Marking * getInitialMarking();
	//void setInitialMarking(const Marking & m);
	int 	getNumMarking();
	void 	buildGraph(Marking & root);
	void 	print_dot(char * ofile);
	void 	check_reachable_place(Place & p);
	int		check_exist(const Marking & m);
	int 	getPostInMset(const Marking & m);
	void 	addtodot(std::string & st, const Marking & a, const Marking & b, const Transition & t); //add an edge from a to b with the label t


}; // end of class

} //end of namespace
#endif /* GRAPH_H_ */
