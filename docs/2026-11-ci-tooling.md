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

- Buildbot

- Github Actions (interested in this one)

- Azure DevOps

##### Tooling

- Docker


### Decision

(Decision still pending, would like to make the decision with Johann)

### Open questions

- How specifically to setup and customize Github Actions

- What do people on the team specifically want, may have to change design depending on what

- 
