from brian2 import *
from simulation_engine import run_simulation
from data_analysis import analyze_firing_rates
from visualization import plot_firing_rates

def main():
    # Run the simulation
    net = run_simulation(duration=10*second)
    
    # Extract the neurons from the network
    neurons = None
    for obj in net.objects:
        if isinstance(obj, NeuronGroup):
            neurons = obj
            break
    
    if neurons is None:
        raise ValueError("NeuronGroup not found in the simulation network.")
    
    # Setup SpikeMonitor
    spikemon = SpikeMonitor(neurons)
    net.add(spikemon)
    
    # Run the network
    net.run(10*second)

    # Analyze and plot firing rates
    firing_rates = analyze_firing_rates(spikemon)
    plot_firing_rates(firing_rates)

if __name__ == "__main__":
    main()
    

