/*
 * Event.h
 *
 *  Created on: 24 lis 2020
 *      Author: Khaless
 */

#ifndef EVENT_H
#define EVENT_H


// ROOT
#include "TObject.h"


class Event : public TObject {
public:
	Event() {}
  //Event(const Event& ups);
  ~Event()	{}

  // event
  float mass;
  float pt;
  float eta;
  float phi;
  float deltaPhi;
  float accepted;


private:
  ClassDef(Event,1);
};



#endif /* EVENT_H */
