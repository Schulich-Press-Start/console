# CI platform and build setup for simulator

### Problem

Without proper CI integration, a broken PR could reach the review phase instead of being caught early, and without proper build tools, the Game design team has no easily accessible way to get a simulator build.

So, we need tooling and continuous integration for error checking code before it is sent to final review. This needs to be automatic and constant.



### Constraints

- Unsure of platform

- Needed to be used by Embedded Software and potentially Game Design

- Must be automatic 

- Must be integrated into Github

- Free/cheap (student team)

- Low maintenance due to small team size

- Able to run on Ubuntu

### Options

##### CI

- Jenkins

- Buildbot (this and jenkins are probably actually a no-go due to requiring server hosting)

- Github Actions (interested in this one, easy and closely integrated with github)

- Azure DevOps 

##### Tooling

- Docker

- 

### Decision

Right now, the decision is pending because I would like Johanns input.

However, from the information I have gathered, it seems that Github Actions is the right choice here for CI.
This is because of the fact it is free, its close integration with Github, and being relatively low maintenance.

### Open questions

- How should the chosen CI workflow be setup?

- What do people on the team specifically want, may have to change design depending on what

- Where should the workflows live?

- 
