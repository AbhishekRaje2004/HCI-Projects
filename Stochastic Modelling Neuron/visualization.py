import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np  # Importing NumPy

def plot_firing_rates(firing_rates):
    """Plot the firing rates as a bar chart."""
    plt.figure(figsize=(10, 6))
    sns.barplot(x=np.arange(len(firing_rates)), y=firing_rates)
    plt.xlabel('Neuron Index')
    plt.ylabel('Firing Rate (Hz)')
    plt.title('Firing Rates of Neurons')
    plt.show()

