/*!
 * \file  marking.cc
 */

#include "config.h"

#include "pnapi-assert.h"

#include "petrinet.h"
#include "marking.h"
#include "util.h"

#ifndef NDEBUG
#include <iostream>

using std::cout;
using std::endl;
#endif /* NDEBUG */

using std::set;
using std::map;


namespace pnapi
{

/****************************************************************************
 *** Class Marking Function Definitions
 ***************************************************************************/

/*!
 * \brief   Standard Constructor
 *
 * \note This constructor does not instantiate a Petri net object.
 */
Marking::Marking() : net_(NULL), rel_()
{
	name_="";
	//rel_= std::vector <const Marking * >();
}


/*!
 * \brief   Constructor for marking class
 *
 * Reads marking from the Petri net.
 *
 * \param n underlying petri net      
 * \param empty  if true, initialize marking to empty marking
 *               instead of reading marking from n
 */
Marking::Marking(PetriNet & n, bool empty) :
  net_(&n),name_("M0"),  rel_()
{
	//rel_=std::vector <Marking * >();
	PNAPI_FOREACH(p, n.getPlaces())
	{
		m_[*p] = empty ? 0 : ((*p)->getTokenCount());
	}
}


/*!
 * \brief   Copy-constructor for marking class
 *
 * \param   m the other Marking
 */
Marking::Marking(const Marking & m) :
  m_(m.m_), net_(m.net_),name_(m.name_)

{
	rel_=m.rel_;
	//for (std::vector<Marking *>::iterator i=m.rel_.begin(); i!=m.rel_.end();++i)
		//rel_.at(i) = m.rel_.at(i);
}


/*!
 * \brief   Another constructor.
 */
Marking::Marking(std::map<const Place *, unsigned int> m, PetriNet * net) :
  m_(m), net_(net), name_(""),  rel_()
{
	//rel_=std::vector <Marking * >();

}

/*!
 * \brief constructor for cross-net-copying
 */
Marking::Marking(const Marking & m, PetriNet * net, 
                  std::map<const Place *, const Place *> & placeMap) :
  net_(net), name_(m.name_),  rel_()
{
  PNAPI_FOREACH(it, m.m_)
  {
    m_[placeMap[it->first]] = it->second;
  }
 // rel_=std::vector <Marking * >();
}

void Marking::setName(const std::string & st)
{
	name_.assign(st);
}

/*!
 * \brief   Returns the map..
 */
const std::map<const Place *, unsigned int> & Marking::getMap() const
{
  return m_;
}

/*!
 * \brief   Returns the Petri net laying under the marking
 */
PetriNet & Marking::getPetriNet() const
{
  return *net_;
}

std::map<const Place *, unsigned int>::const_iterator Marking::begin() const
{
  return m_.begin();
}

std::map<const Place *, unsigned int>::const_iterator Marking::end() const
{
  return m_.end();
}

/*!
 * \brief   Returns the size of the Marking
 */
unsigned int Marking::size() const
{
  return m_.size();
}


/*!
 * \brief   Decides if this marking activates transition t
 *
 * \param   t the Transition in question
 *
 * \return  true iff this activates t
 */
bool Marking::activates(const Transition & t) const
{
  PNAPI_FOREACH(f, t.getPresetArcs())
  {
    map<const Place *, unsigned int>::const_iterator it = m_.find(static_cast<Place *>(&(*f)->getSourceNode()));
    if((it == m_.end()) || ((**f).getWeight() > it->second))
    {
      return false;
    }
  }

  return true;
}


/*!
 * \brief   Calculates the successor marking under transition t
 *
 * \param   t Transition to be fired
 *
 * \return  direct successor of this marking under t
 * 
 * \note successor will be allocated on the heap;
 *       you have to delete it, when you don't need it anymore
 * 
 * \todo review this and Automaton::dfs() to avoid memory leaks
 */
Marking & Marking::getSuccessor(const Transition & t) const
{
  set<Node *> Ppre = t.getPreset();
  set<Node *> Ppost = t.getPostset();
  Marking & m = *new Marking(*this);

  PNAPI_FOREACH(p, Ppre)
  {
    m[*static_cast<Place *>(*p)] -= net_->findArc(**p, t)->getWeight();
  }

  PNAPI_FOREACH(p, Ppost)
  {
    m[*static_cast<Place *>(*p)] += net_->findArc(t, **p)->getWeight();
  }

  m.rel_.clear();
  return m;
}

std::string Marking::getName() const
{
	return name_;
}

/*!
 * \brief   overloaded operator [] for Markings
 */
unsigned int & Marking::operator [](const Place & offset)
{
  return m_[&offset];
}
/*!
 * return all relevant marking (successors) of a marking
 */
std::vector <std::string> & Marking::getRelMarkings()
{
	return rel_;
}
/*!
 * \brief   overloaded operator [] for Markings
 */
unsigned int Marking::operator[](const Place & p) const
{
  map<const Place *, unsigned int>::const_iterator it = m_.find(&p); 

  return ((it == m_.end()) ? 0 : it->second);
}


/*!
 * \brief   overloaded operator == for Markings
 */
bool Marking::operator==(const Marking & m) const
{
  return (m_ == m.getMap());
}


/*!
 * \brief   overloaded assignment operator
 */
Marking & Marking::operator=(const Marking & m)
{
  PNAPI_ASSERT(this != &m);
  
  m_ = std::map<const Place *, unsigned int>(); // fixing cppcheck error
 // name_=m.name_;
  rel_.clear();
  //net_=m.net_;
  this->~Marking();
  return *new Marking(m);// *new (this) Marking(m);
}


/*!
 * \brief   clears the marking
 */
void Marking::clear()
{
  m_.clear();
}

/*!
 * get all activated transition from the current marking
 */
std::set<Transition *>  Marking::getEnabledTransitions() const
{
	std::set<Transition *> tt ;
	PNAPI_FOREACH (t,net_->getTransitions())
	{
		if (this->activates(**t))
			tt.insert(*t);
	}
	return tt;
}
/*!
 * add new successor to a marking.
 */
void Marking::addSuccessor (std::string id)
{
	rel_.push_back(id);

}
/*!
 * check if place hold a token or not
 */
bool Marking::check_hodingplace(Place & p)
{
	PNAPI_FOREACH (it,net_->getPlaces())
		if (p.getName().compare((*it)->getName()))
			if ((*it)->getTokenCount()!=0)
				return true;
	return false;
}
/*!
 * copy relative markings of m to current marking
 */

void Marking::copyRel(Marking & m)
{
	rel_.insert(rel_.end(),m.rel_.begin(),m.rel_.end());
}

} /* namespace pnapi */

