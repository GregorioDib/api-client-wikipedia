# WORKPLAN

## Project Goal

Develop a modular worker application in C capable of consuming the Wikipedia public API, processing JSON responses, and persisting article metadata into a relational SQL database.

The project followed an incremental development process, where each milestone produced a functional and compilable version of the application.

---

# Development Strategy

The implementation was divided into small, functional milestones.

Each milestone:

* produced a compilable project;
* introduced one major feature;
* was committed independently to Git;
* kept the repository in a working state.

The commits represented completed functionalities rather than individual source files.

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

## Milestone 5 — Worker Orchestration [DONE]

**Objective**

Integrate all modules into a complete worker pipeline.

**Deliverables**

* Command-line argument handling
* Complete execution flow
* Metadata persistence
* Storage of the complete JSON payload into the database

---

## Milestone 6 — Asset History Support [DONE]

**Objective**

Preserve historical versions of stored assets.

**Deliverables**

* Asset update detection
* Snapshot generation
* Persistence into the `assets_history` table
* History management logic

---

## Milestone 7 — Validation and Documentation [DONE]

**Objective**

Prepare the project for delivery.

**Deliverables**

* Functional testing
* Code cleanup
* Documentation updates
* README revision
* Final verification under Linux

---

# Final Result

The completed application is capable of:

* querying the Wikipedia public REST API;
* processing JSON responses;
* extracting relevant metadata;
* persisting assets into the database;
* preserving historical versions of modified assets;
* providing a modular and maintainable codebase suitable for future extensions.
