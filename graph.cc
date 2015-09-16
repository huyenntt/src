/*
 * graph.cpp
 *
 *  Created on: 11 sept. 2015
 *      Author: tnguyen
 */
#include "config.h"
#include "util.h"


#include <iostream>
#include  <vector>

#include "graph.h"
#include "component.h"


using std::cout;
using std::endl;
using std::map;
using std::vector;


namespace pnapi
{

Graph::Graph() {
	// TODO Auto-generated constructor stub
	init_=new Marking();
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
	init_=NULL;
}

Graph::Graph(Marking & m): init_(&m)
{

}


/*Marking * Graph::getInitialMarking()
{
	return init_;
}

void Graph::setInitialMarking(const Marking & m)
{
	*init_= m;
}
*/

//----get the number of markings in the graph
int Graph::getNumMarking()
{
	return Mset_.size();
}

/*!
 * check if marking m exist in Mset of the graph or not
 * @param m
 * @return
 */

bool Graph::check_exist(const Marking & m)
{
	for (int i=0; i<Mset_.size(); ++i)
		if (m==Mset_.at(i))
			return true;

	return false;
}

int Graph::getPostInMset(const Marking & m)
{
	for (int i=0; i<Mset_.size(); ++i)
		if (m==Mset_.at(i))
			return i;
	return 0;

}
/*!
 * add all succssor of marking m to Mset of the graph
 * as well as to list of relative markings rel_ for marking m.
 * @param m
 */
void Graph::buildGraph(Marking & root)
{
	std::set<Transition *> at=root.getEnabledTransitions();
	//std::cout<<"Number of activate transitions: "<< at.size()<<std::endl;
	if (at.empty())	return;

	std::set<Transition *>::iterator it;
	for (it=at.begin();it!=at.end();++it)
	{
		/*
		if (!check_exist(root.getSuccessor(**it)))
		{
			Mset_.push_back(root.getSuccessor(**it));
			//Marking & tm=Mset_.at(Mset_.size()-1);
			//root.addSuccessor(tm);
			buildGraph(root.getSuccessor(**it));
		}
		*/
		Marking & temp=root.getSuccessor(**it);
		if (!check_exist(temp))
		{
			Mset_.push_back(temp);
			root.addSuccessor(temp);
			buildGraph(temp);
		}
		else
		{
			//int i=this->getPostInMset(temp);
			root.addSuccessor(temp);
		}
	}

	return ;
} // end of function add marking
/*!
 * check if place p is reachable or not (it has some tokens in a reachable marking)
 */
void Graph::check_reachable_place(Place & p)
{
	PNAPI_FOREACH(it, Mset_)
		if (it->check_hodingplace(p))
		{
			std::cout<<"P is reachable"<<std::endl;
			return;
		}
	std::cout<<"P is unreachable"<<std::endl;
}
/*!
 * use io::dot to print out the graph with Graphviz
 */
void 	print_dot()
{
//std::cout<<io::dot<<*this;
}


} //end of pnapi

