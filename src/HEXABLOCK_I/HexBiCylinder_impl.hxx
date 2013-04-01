// Copyright (C) 2009-2013  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#ifndef __HexBiCylinder_impl__
#define __HexBiCylinder_impl__

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(BiCylinder)
#include "SALOME_GenericObj_i.hh"

#include "hexa_base.hxx"
#include "HexBiCylinder.hxx"

using namespace HEXABLOCK_ORB;

class BiCylinder_impl : public POA_HEXABLOCK_ORB::BiCylinder,
		        public SALOME::GenericObj_i
{
public:
    BiCylinder_impl(HEXA_NS::BiCylinder *ptrCpp);
    HEXA_NS::BiCylinder* GetImpl();

    Vertex_ptr getVertexIJK(::CORBA::Long part,::CORBA::Long x,::CORBA::Long y,::CORBA::Long z)
              throw (SALOME::SALOME_Exception);

    Edge_ptr getEdgeI(::CORBA::Long part, ::CORBA::Long x, ::CORBA::Long y, ::CORBA::Long z)
             throw (SALOME::SALOME_Exception);
    Edge_ptr getEdgeJ(::CORBA::Long part, ::CORBA::Long x, ::CORBA::Long y, ::CORBA::Long z)
             throw (SALOME::SALOME_Exception);
    Edge_ptr getEdgeK(::CORBA::Long part, ::CORBA::Long x, ::CORBA::Long y, ::CORBA::Long z)
             throw (SALOME::SALOME_Exception);

    Quad_ptr getQuadIJ(::CORBA::Long part, ::CORBA::Long x, ::CORBA::Long y, ::CORBA::Long z)
             throw (SALOME::SALOME_Exception);
    Quad_ptr getQuadIK(::CORBA::Long part, ::CORBA::Long x, ::CORBA::Long y, ::CORBA::Long z)
             throw (SALOME::SALOME_Exception);
    Quad_ptr getQuadJK(::CORBA::Long part, ::CORBA::Long x, ::CORBA::Long y, ::CORBA::Long z)
             throw (SALOME::SALOME_Exception);

    Hexa_ptr getHexaIJK(::CORBA::Long part, ::CORBA::Long x, ::CORBA::Long y, ::CORBA::Long z)
             throw (SALOME::SALOME_Exception);

  ::CORBA::Long saveVtk(const char* fname) throw (SALOME::SALOME_Exception);
  void dump() throw (SALOME::SALOME_Exception);
  void printName() throw (SALOME::SALOME_Exception);
  char* getName() throw (SALOME::SALOME_Exception);
  void  setName (const char* name) throw (SALOME::SALOME_Exception);

private:
    HEXA_NS::BiCylinder* bi_cylinder_cpp;
};

#endif
