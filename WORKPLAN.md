# WORKPLAN

## Project Goal

Develop a modular worker application in C capable of consuming the Wikipedia public API, processing JSON responses, and persisting article metadata into a relational SQL database.

The project is intended to follow an incremental development process, where each milestone produces a functional and compilable version of the application.

The provisional target completion date is **July 28, 2026**, allowing approximately one week before the planned project presentation.

---

# Development Strategy

The implementation will be divided into small, functional milestones.

Each milestone should:

* produce a compilable project;
* introduce one major feature;
* be committed independently to Git;
* keep the repository in a working state.

Commits will represent completed functionalities rather than individual source files.

---

# Milestones

## Milestone 1 — Project Skeleton [DONE]

**Objective**

Create the initial project structure.

**Deliverables**

* Repository organization
* Source and include directories
* Makefile
* Main entry point
* Configuration header
* Initial build verification

---

## Milestone 2 — Database Layer [DONE]

**Objective**

Implement database initialization and persistence infrastructure.

**Deliverables**

* SQL schema integration
* Database connection module
* Database initialization
* Table creation
* Connection management

---

## Milestone 3 — Wikipedia API Client [DONE]

**Objective**

Implement communication with the Wikipedia REST API.

**Deliverables**

* HTTP client implementation
* Request execution
* Response retrieval
* Error handling for network failures

---

## Milestone 4 — JSON Processing [DONE]

**Objective**

Process Wikipedia API responses.

**Deliverables**

* JSON parsing
* Metadata extraction
* Validation of required fields
* Internal data representation

---

## Milestone 5 — Worker Orchestration

**Objective**

Integrate all modules into a complete worker pipeline.

**Deliverables**

* Command-line argument handling
* Complete execution flow
* Metadata persistence
* Storage of the complete JSON payload into the database

---

## Milestone 6 — Asset History Support

**Objective**

Preserve historical versions of stored assets.

**Deliverables**

* Asset update detection
* Snapshot generation
* Persistence into the `assets_history` table
* History management logic

---

## Milestone 7 — Validation and Documentation

**Objective**

Prepare the project for delivery.

**Deliverables**

* Functional testing
* Code cleanup
* Documentation updates
* README revision
* Final verification under Linux

---

# Repository Workflow

The repository will evolve through small, incremental commits.

Each commit should correspond to one completed functionality instead of isolated file modifications.

Every milestone should leave the project in a compilable and executable state.

---

# Expected Final Result

At the end of the project, the worker will be capable of:

* querying the Wikipedia public REST API;
* processing JSON responses;
* extracting relevant metadata;
* persisting assets into the database;
* preserving historical versions of modified assets;
* providing a modular and maintainable codebase suitable for future extensions.
