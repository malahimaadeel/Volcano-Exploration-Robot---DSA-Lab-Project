# Volcano Exploration Robot — Mission Control System

DSA Lab project. Simulates mission control for robots exploring volcano hazard zones.
Combines 4 core data structures in one working system.

**Bahria University Karachi Campus**
Course: DSA Lab

## Data Structures Used

| Structure | Used For |
|---|---|
| **BST (Binary Search Tree)** | Zone manager — zones ranked by danger ID, insert/search/delete/inorder traversal |
| **Linked List** | Robot roster — add/remove robots, track battery, sensor type, status |
| **Queue (with priority)** | Mission queue — zones wait in line for dispatch, emergency zones jump to front |
| **Stack** | Path tracker — records robot's path into a zone so it can retreat (LIFO) |

## Features
- Insert / search / delete hazard zones (BST)
- Add / remove robots with live battery + sensor info
- Dispatch robot from mission queue, auto-assign to standby robot
- Priority queue jump for urgent zones
- Stack-based path tracking so robot can retreat step by step
- Full memory cleanup on exit (no leaks — BST, list, queue, stack all freed)

## Menu Overview
- 1-4: Zone Manager (BST)
- 5-8: Robot Manager (Linked List)
- 9-12: Mission Queue
- 13-15: Path Tracker (Stack)
- 0: Exit (cleans up memory)

## Why This Project
Real-world inspired: disaster-response robots need to track hazard zones,
manage limited robot resources, prioritize urgent missions, and retrace
their path back to safety. Each DSA structure maps to a real operational need.
