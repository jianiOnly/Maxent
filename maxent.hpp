/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#pragma once

#include <fstream>
#include "maxent_matrix_def.hpp"
#include "maxent_parms.hpp"

#include"gtest/gtest.h"

class MaxEntHelper : protected MaxEntParameters
{
public : 

  MaxEntHelper(alps::params& p);

  double omega_coord(const int i) const { return MaxEntParameters::omega_coord(i); }

  ///getter function for the discretized default model
  double Default(const int i) const { return def_[i]; }  
  ///getter function for the discretized default model
  const vector_type& Default() const { return def_; }

  //compute the 'free energy' Q (eq. 6.8 in Sebastian's thesis) according to equation D.8 in Sebastian's thesis
  virtual double Q(const vector_type& u, const double alpha) const {
    vector_type A=transform_into_real_space(u);
    return 0.5*chi2(A)-alpha*entropy(A);
  }

  ///number of points for the Matsubara data
  int ndat() const { return MaxEntParameters::ndat(); }

  ///A->u
  vector_type transform_into_singular_space(vector_type A) const;
  ///u-A
  vector_type transform_into_real_space(vector_type u) const;
  ///A=A_i/\sigma_i; this removes \sigma_i
  vector_type get_spectrum(const vector_type& u) const;
  vector_type PrincipalValue(const vector_type &w,const vector_type &a) const;
  /// \Sigma*(V^T*RealSpace(u)*V)*\Sigma
  virtual matrix_type left_side(const vector_type& u) const;
  /// \Sigma*U^T*(K*RealSpace(u)-y)
  virtual vector_type right_side(const vector_type& u) const;
  ///  \delta \dot (V^T*RealSpace(u)*V)
  double step_length(const vector_type& delta, const vector_type& u) const;
  virtual double convergence(const vector_type& u, const double alpha) const;
  double log_prob(const vector_type& u, const double alpha) const;
  double chi_scale_factor(vector_type A, const double chi_sq, const double alpha) const;
  double chi2(const vector_type& u) const;
  void print_chi2(const vector_type& u, std::ostream &os) const;
  /// compute S=\int d\omega(A(\omega)-D(\omega)-A(\omega)\ln[A(\omega)/D(\omega))
  double entropy(const vector_type& u) const;

private:
  ///discretized and normalized version of the default model.
  vector_type def_;
  ///check that the default model is non-zero
  void checkDefaultModel(const vector_type &D) const;
};




class MaxEntSimulation : protected MaxEntHelper
{

public:

  ///setup of parameters
  MaxEntSimulation(alps::params& parms);
  ///the maxent calculation
  void run();
  ///the evaluation and writing of files
  void evaluate();
  vector_type levenberg_marquardt(vector_type u, const double alpha) const;
  vector_type iteration(vector_type u, const double alpha, const double mu) const;
  ///define parameter defaults
  static void define_parameters(alps::params &p);

private:

  ///grid of alpha values
  vector_type alpha;
  const double norm;
  const int max_it;
  std::string name,Kernel_type;
  bool verbose,text_output,self;
  const int nfreq;

  vector_type lprob;
  vector_type chi_sq;
  std::vector<vector_type> spectra;
  vector_type u;
  ///averaged spectrum
  vector_type avspec;
  ///classic MaxEnt
  vector_type maxspec;
  ///grid of Omega points
  vector_type omegaGrid;
  //posterior probability of the default model
  double postprobdef;
  ///vector of calculated Q values for each alpha iteration
  //this is one method of checking if a minimum was found
  vector_type qvec;

public:
  ///getter for avspec, the averaged spectrum
  const vector_type getAvspec() const{return avspec;}
  ///getter for maxspec, the most probable spectrum
  const vector_type getMaxspec() const{return maxspec;}
  ///getter for the grid of omega points used in determining 
  ///the spectral function A(omega)
  const vector_type getOmegaGrid() const{return omegaGrid;}
  ///getter for the posterior probability of the default model
  const double getPostProb() const{return postprobdef;}
  ///getter for alpha values
  const vector_type getAlphaGrid() const{return alpha;} 
  ///getter for vector of Q value per alpha iteration
  const vector_type getQvec() const{return qvec;}
}; 

///Real time simulation class, implements extra term \gamma*R^2
class MaxEntSimulationRT : public MaxEntSimulation{
public:
  ///setup of parameters
  MaxEntSimulationRT(alps::params& parms);
  
private:
  //these are all virtual functions that are modified slightly 
  //due to the \gamma*R^2 term
  vector_type right_side(const vector_type& u) const;
  matrix_type left_side(const vector_type& u) const;
  double Q(const vector_type& u, const double alpha) const;
  double convergence(const vector_type& u, const double alpha) const;
  double rt_chi(const vector_type& u) const;
  ///gamma for RT points
  const double GAMMA;
}; 
