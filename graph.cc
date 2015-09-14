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
	init_=NULL;
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
	init_=NULL;
}

Graph::Graph(Marking m)
{
	init_=&m;
//	std::vector<Marking>::iterator it=Mset_.begin();
	Mset_.push_back(m);
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
 * add all succssor of a marking m to Mset of the graph
 * as well as to list of relative markings rel_ for marking m.
 * @param m
 */
void Graph::addMarking(Marking & m)
{
	std::set<Transition *> at=m.getActivateTransitions();
	std::cout<<"Number of activate transitions: "<< at.size()<<std::endl;
	if (at.empty())	return;

	std::set<Transition *>::iterator it;
	for (it=at.begin();it!=at.end();++it)
	{
		Marking & temp= m.getSuccessor(**it);

		if (!check_exist(temp))
		{
			Mset_.push_back(temp);
			m.addSuccessor(*Mset_.end());
			addMarking(temp);
		}

		else
		{
			int i=this->getPostInMset(temp);
			//m.addSuccessor(Mset_.at(i));
		}
	}

	return ;
} // end of function add marking

} //end of pnapi

