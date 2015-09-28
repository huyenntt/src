/*
 * graph.cpp
 *
 *  Created on: 11 sept. 2015
 *      Author: tnguyen
 */
#include "config.h"
#include "util.h"


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stack>

#include "graph.h"
#include "component.h"


using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::fstream;
using std::ostringstream;
using std::stack;


namespace pnapi
{

Graph::Graph(){
	// TODO Auto-generated constructor stub
	//init_=new Marking();
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
	//init_=NULL;
}

Graph::Graph(const Marking & m)
{
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
	/*std::vector<Marking>::iterator it;
	for (it=Mset_.begin(); it!=Mset_.end(); ++it)
		cout<<(*it).getName()<<"-->";
	*/
	return Mset_.size();
}

/*!
 * check if marking m exist in Mset of the graph or not
 * @param m
 * @return
 */

signed int Graph::check_exist(const Marking & m)
{
	if (Mset_.size()==0) return -1;
	else
		for (int i=0; i<Mset_.size(); ++i)
		if (m==Mset_.at(i))
			return i;
	return -1;
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
	std::stack <Marking> mstack;
	//std::string st = "digraph RGraph {\n";
	mstack.push(root);

	while (!mstack.empty())
	{
		Marking curr = mstack.top();
		//cout <<"current marking is: "<<curr.getName();
		mstack.pop();
		std::set<Transition *> at=curr.getEnabledTransitions();
		if (at.empty())
			continue;
		else
		{
		//	cout<<"\nThere are "<< at.size()<<" enabled transitions \n";
			int count =0;
			std::set<Transition *>::iterator it;
			for (it = at.begin();it != at.end(); ++it)
			{
				Marking temp = curr.getSuccessor(**it);
				if ((check_exist(temp)!= -1)||(curr==temp))
				{
					std::string id=(Mset_.at(check_exist(temp))).getName();
					//	cout<<"sucessor is old marking "<<id;
					curr.addSuccessor(id);
				}
				else
				{
					count++;
					//set name = current marking name + count
					ostringstream ss;
					ss << count;
					temp.setName(curr.getName()+ "_"+ ss.str());
					//cout<<" name of marking is: "<<temp.getName()<<"\n";
					//addtodot(st,curr,temp,**it);
					mstack.push(temp);
					Mset_.push_back(temp);
					std::string id=(Mset_.back()).getName();
					curr.addSuccessor(id); // add reference to item just pushed back
				}
			} // end of for
			if (check_exist(curr)<0)
				Mset_.push_back(curr);
			else
			{
				signed int i= check_exist(curr);
				Mset_.at(i).copyRel(curr);
			}
		} // end of if

	} // end of while

/*	fstream fs("abc.dot",fstream::out);
	fs << st<< "}\n";
	fs.close();*/
	return ;
} // end of function buildGraph

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
 * print graph to a dot file.
 */
void Graph::print_dot(char * ofile)
{
	fstream fs(ofile,fstream::out);
	fs << "Digraph RGraph {\n";
	std::vector<Marking>::iterator i;
	  for(i=Mset_.begin();i!=Mset_.end();++i)
	  {
		std::vector<std::string> & rs = (*i).getRelMarkings();
		if (rs.size()!=0)
		{
			std::vector<std::string>::iterator im;
			std::string ranksame= "{rank=same; " ;
			for (im = rs.begin(); im!=rs.end(); ++im)
			{
				fs << (*i).getName() << "->";
				fs << *im << ";\n";
				ranksame+= *im + "; ";
			}

		//	fs<<ranksame<< "}\n";
		}
	  }
	fs<< "}";
	fs.close();
}

/*
 * Add an edge (a -t-> b) into file dot
*/
/*
void Graph::addtodot(std::string & st,const Marking & a, const Marking & b, const Transition & t)
{
	st += a.getName() + "->" + b.getName() + "[label=\"" + t.getName() + "\",style=plaintext ]; \n";
}
*/
} //end of pnapi

