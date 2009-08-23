/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */


#include "swordorb-impl.hpp"
#include <iostream>
#include <swmgr.h>
#include "../orbitcpp/webmgr.hpp"

SWConfig *sysConf = 0;
WebMgr *swordMgr = 0;

class CleanStatics {
public:
	CleanStatics() {}
	~CleanStatics() {
		if (swordMgr)
			delete swordMgr;

		if (sysConf)
			delete sysConf;
	}
} cleanStatics;




int main (int argc, char** argv)
{
  try {

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-sysConf")) {
			if ((i+1) < argc)
				sysConf = new SWConfig(argv[i+1]);
		}
	}

	swordMgr = new WebMgr(sysConf);


	const char* options[][2] = {
		{ "nativeCharCodeSet", "UTF-8" }
	   , { 0, 0 }
     };


    // Initialise the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "omniORB4", options);

    // Obtain a reference to the root POA.
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

    // We allocate the objects on the heap.  Since these are reference
    // counted objects, they will be deleted by the POA when they are no
    // longer needed.
//    swordorb_SWModule_i* myswordorb_SWModule_i = new swordorb_SWModule_i();
    swordorb_SWMgr_i* myswordorb_SWMgr_i = new swordorb_SWMgr_i(swordMgr);


    // Activate the objects.  This tells the POA that the objects are
    // ready to accept requests.
 //   PortableServer::ObjectId_var myswordorb_SWModule_iid = poa->activate_object(myswordorb_SWModule_i);
    PortableServer::ObjectId_var myswordorb_SWMgr_iid = poa->activate_object(myswordorb_SWMgr_i);


    {
      // IDL interface: swordorb::SWMgr
      CORBA::Object_var ref = myswordorb_SWMgr_i->_this();
      CORBA::String_var sior(orb->object_to_string(ref));
      std::cout << (char*)sior << std::endl;
    }



    // Obtain a POAManager, and tell the POA to start accepting
    // requests on its objects.
    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    orb->run();
    orb->destroy();
  }
  catch(CORBA::TRANSIENT&) {
    std::cerr << "Caught system exception TRANSIENT -- unable to contact the "
         << "server." << std::endl;
  }
  catch(CORBA::SystemException& ex) {
    std::cerr << "Caught a CORBA::" << ex._name() << std::endl;
  }
  catch(CORBA::Exception& ex) {
    std::cerr << "Caught CORBA::Exception: " << ex._name() << std::endl;
  }
  catch(omniORB::fatalException& fe) {
    std::cerr << "Caught omniORB::fatalException:" << std::endl;
    std::cerr << "  file: " << fe.file() << std::endl;
    std::cerr << "  line: " << fe.line() << std::endl;
    std::cerr << "  mesg: " << fe.errmsg() << std::endl;
  }
  return 0;
}

