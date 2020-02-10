ETD2_HH
======

This code is for evolving pulse-coupled Hodgkin-Huxley (HH) neuronal network. 
We introdce three second-order numerical methods, Runge-Kutta (RK), exponentional time differencing (ETD2), and adaptive ETD (AETD2) methods. 
The RK2 method should use a small time step on the order of 0.02 ms to satisfy the requirement of stability and is offerred as a banchmark.
The ETD2 method can use a large time step to improve the numerical efficiency, but it is less accurate than the RK2 method.
The AETD2 method combines the advantages of the RK2 and ETD2 methods that it can use a large time step (maximum 0.27 ms) to evolve the HH network while achieving comparable high accuracy as the RK2 method using a small time step. 
The detailed method can be found in reference. 

# lalalal
thank 'you'




References 
============
Tian ZQK, Zhou D. Exponential time differencing algorithm for pulse-coupled hodgkin-huxley neuronal networks. arXiv preprint arXiv:1910.08724 (2019). 

Börgers C, Nectow AR. Exponential time differencing for hodgkin–huxley-like odes. SIAM Journal on 
Scientific Computing 35 (2013) B623–B643. 

CoxSM,MatthewsPC. Exponentialtimedifferencingforstiffsystems. JournalofComputationalPhysics(2002) 430–455. 

Hansel D, Mato G, Meunier C, Neltner L. On numerical simulations of integrate-and-fire neural networks. 
Neural Computation 10 (1998) 467–483. 

Shelley MJ, Tao L. Efficient and accurate time-stepping schemes for integrate-and-fire neuronal networks. 
Journal of Computational Neuroscience 11 (2001) 111–119. 

