from brian2 import *

def create_neural_network(num_neurons=100, connectivity=0.1):
    """Create a network using the Brian2 library."""
    start_scope()
    
    # Define model parameters directly in the equations
    eqs = '''
    dv/dt = (1-v)/tau : 1 (unless refractory)
    tau = 10*ms : second  # Define tau directly in the equations to ensure it is recognized
    '''
    
    # Create NeuronGroup with equations that include tau
    neurons = NeuronGroup(num_neurons, eqs, threshold='v > 0.8', reset='v = 0', refractory=5*ms, method='exact')
    
    # Define synapses
    synapses = Synapses(neurons, neurons, on_pre='v_post += 0.2')
    synapses.connect(p=connectivity)
    
    return neurons, synapses


