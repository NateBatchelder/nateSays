import random

# List of possible colors
colors = ['red', 'blue', 'green', 'yellow']

# Generate a random sequence of colors
sequence = [random.choice(colors) for _ in range(10)]

# Function to play the game
def simon_says():
    print("Simon says: Repeat the sequence!")
    for color in sequence:
        # Display the color
        print(color)
        # Wait for user input
        input("Press Enter to continue...")
    print("Simon says: Well done!")

# Start the game
simon_says()
