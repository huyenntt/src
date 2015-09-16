// -*- C++ -*-

/*!
 * \file  marking.h
 */

#ifndef PNAPI_MARKING_H
#define PNAPI_MARKING_H

#include "config.h"

#include <map>
#include <vector>

namespace pnapi
{

// forward declarations
class PetriNet;
class Transition;
class Place;


/*!
 * \brief   Marking of all places of a net
 *
 * \note    In the future this class might contain a highly efficient
 *          implementation if necessary. For now, we use a simple one to
 *          determine the needed functionality.
 */
class Marking
{
private: /* private variables */
  std::map<const Place *, unsigned int> m_;
  PetriNet * net_;
  std::vector<const Marking *> rel_;  // store successor markings in the reachability graph.
  std::string name_;
public: /* public methods */
  /*!
   * \name constructors and destructors
   */
  //@{
  /// constructor
  Marking();
  /// constructor
  Marking(PetriNet &, bool = false);
  /// copy constructor
  Marking(const Marking &);
  /// constructor
  Marking(std::map<const Place *, unsigned int>, PetriNet * = NULL);
  /// constructor
  Marking(const Marking &, PetriNet *, std::map<const Place *, const Place *> &);
  /// destructor
  virtual ~Marking() {}
  /// assignment operator
  Marking & operator=(const Marking &);
  //@}
  // set name to a marking
  void setName(const std::string &);


  /*!
   * \name structural changes
   */
  //@{
  /// clear this marking
  void clear();
  //@}
  
  /*!
   * \name getter
   */
  //@{
  /// get mapping from places to their marking
  const std::map<const Place *, unsigned int> & getMap() const;
  /// get iterator to the first element in the marking map
  std::map<const Place *, unsigned int>::const_iterator begin() const;
  /// get end-iterator of the marking map
  std::map<const Place *, unsigned int>::const_iterator end() const;
  /// get size of the marking map
  unsigned int size() const;
  /// get unterlying petri net
  PetriNet & getPetriNet() const;
  /// whether this marking is equal to a given other marking
  bool operator==(const Marking &) const;
  /// whether marking activates a given transition
  bool activates(const Transition &) const;
  /// get the successor by fireing a given transition
  Marking & getSuccessor(const Transition &) const;
  ///get name of a Marking
  std::string getName() const;

  /// get all transitions enabled from marking m
  std::set<Transition *> getEnabledTransitions() const;
  /// add a succeesor m to current marking
  void addSuccessor(const Marking & m);

  /// get writing access to the marking of a given place
  unsigned int & operator[](const Place &);
  /// get the marking of a given place
  unsigned int operator[](const Place &) const;
  //check if place p holds some token or not
  bool check_hodingplace(Place & p);
   //@}
};

} /* namespace pnapi */

#endif /* PNAPI_MARKING_H */
