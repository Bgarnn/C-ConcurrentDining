Philosophers - Dining Philosophers Problem Solver

Welcome to Philosophers, a project that tackles the classic Dining Philosophers problem, exploring synchronization and parallel programming concepts. This project challenges you to implement a solution that allows philosophers to dine peacefully while avoiding conflicts over shared resources, emphasizing concurrent programming skills and system-level understanding.

** Project Overview **

- What does Philosophers do?
    - Philosophers is an implementation of the Dining Philosophers problem, a classic synchronization problem in computer science. The project simulates a scenario where philosophers must contend for shared resources (the dining table) without encountering deadlock or resource contention issues. It serves as a practical exploration of parallel programming and synchronization mechanisms.

- Key Features
    - Concurrency Handling: Philosophers implements a solution to allow philosophers to dine concurrently without conflicts, demonstrating effective synchronization mechanisms.
    - Resource Management: The project explores efficient resource management to ensure philosophers can access shared resources without encountering race conditions or deadlocks.
    - Optimization Challenges: Philosophers may include optimization challenges, such as minimizing resource contention or achieving optimal resource utilization.

** How to Compile Philosophers **

To compile the Philosophers program, follow these straightforward steps:

  - Clone the Repository:
    - git clone git@github.com:Bgarnn/03-Philosophers.git
    - cd 03-Philosophers
  - Compile the Program:
    - make: This will generate the philo executable.
  - Clean the Build:
    - make clean: This removes the object files, leaving only the compiled program.

** Now you can run Philosophers to observe the synchronized dining of philosophers!

** Usage Example

./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

For example:

./philo 5 800 200 200

(This will run the simulation with 5 philosophers, a time to die of 800 milliseconds, a time to eat of 200 milliseconds, and a time to sleep of 200 milliseconds.)
