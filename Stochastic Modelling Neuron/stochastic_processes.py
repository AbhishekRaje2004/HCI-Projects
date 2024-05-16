import numpy as np

def stochastic_activation(neurons, activation_rate=0.01):
    """Randomly activate neurons based on a specified rate."""
    random_activations = np.random.binomial(1, activation_rate, size=len(neurons))
    neurons.v += random_activations


