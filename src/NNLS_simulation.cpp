/*
 * Copyright (C) 1998-2016 ALPS Collaboration
 *
 *     This program is free software; you can redistribute it and/or modify it
 *     under the terms of the GNU General Public License as published by the Free
 *     Software Foundation; either version 2 of the License, or (at your option)
 *     any later version.
 *
 *     This program is distributed in the hope that it will be useful, but WITHOUT
 *     ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *     FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *     more details.
 *
 *     You should have received a copy of the GNU General Public License along
 *     with this program; if not, write to the Free Software Foundation, Inc., 59
 *     Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include <iomanip>
#include "NNLS_method.hpp"
#include <alps/hdf5/vector.hpp>

NNLS_Simulation::NNLS_Simulation(alps::params &parms):
 KernelAndGridIO(parms)
, norm(parms["NORM"])                                             //The integral is normalized to NORM (use e.g. for self-energies
, Kernel_type(parms["KERNEL"].as<std::string>())
, nfreq(parms["NFREQ"].as<int>())
{
  std::string bn=parms["BASENAME"]; name=bn+'.';

  if(norm != 1.) std::cout<<"Data is assumed to be normalized to: "<<norm<<std::endl;

}
///define parameter defaults
void NNLS_Simulation::define_parameters(alps::params &p){
  KernelAndGridIO::define_parameters(p);
  p.description("NNLS - a utility for "
    "performing analytic continuation using non-linear least squares.\n Experimental, do not use.\n");

  p.define<std::string>("BASENAME","","Specified output name \n(generated if not given)");
}

void NNLS_Simulation::run(){
  std::cout<<"This is an NNLS run. "<<std::endl;

  return;
}
void NNLS_Simulation::evaluate(){

  return;
}
