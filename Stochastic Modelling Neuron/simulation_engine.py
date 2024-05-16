from brian2 import *	

from brian2 import run
from neural_network import create_neural_network
from synaptic_dynamics import update_synapses
from stochastic_processes import stochastic_activation

def run_simulation(duration=1*second):
    neurons, synapses = create_neural_network()
    net = Network(neurons, synapses)
    net.run(duration)
    return net


