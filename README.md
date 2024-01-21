# Grid

Modular UEditor tool & runtime plugin for creating grid based experiences in Unreal Engine 5.

https://user-images.githubusercontent.com/68201023/168685543-624972d3-5c92-4345-9f46-bbfe4148984a.mp4

Grid embraces the design philosophy of providing maximum impact without influencing other design decisions.

By leveraging the highly flexible and user friendly Gameplay Tags system Grid allows developers to easily iterate in either Blueprints or C++.

***

### Features

Create and customise 2D and 3D grids using an intuitive UEditor mode.

Separate self-contained workflows (Grid and GridEditor) for easy integration into any project.

Save/Load with a Grid Data Asset (Editor & runtime) for runtime gameplay functionality.

Save/Load with JSON (Editor only) for easy management in all popular version control systems.

***

### Usage

Grid is entirely encapsulated within the **Grid Subsystem** runtime UWorld subsystem and its UEditor plugin.

**Grid Subsystem**

Inherits life cycle of a normal UWorld and contains runtime `Grid(s)`.

**Grid Data Asset(s)**

Authored by GridEditor, with JSON support, and loaded into the GridSubsystem. These data assets define the grid parameters, size, and the tags of each grid square.

**Gameplay Tags**

The example uses `Grid.Interactable` and `Grid.Passable` as a starting point but is intended to be overriden on a case by case basis.

***

Licensed under GPLv3 (c) gnu-enjoyer 2022
