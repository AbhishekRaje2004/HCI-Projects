import numpy as np

def update_synapses(synapses, base_efficiency=0.1, variability=0.03):
    """Update synaptic efficiencies with stochastic variability."""
    new_weights = base_efficiency + np.random.normal(0, variability, size=len(synapses))
    synapses.w = new_weights


