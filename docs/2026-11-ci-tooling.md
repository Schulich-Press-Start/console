# CI platform and build setup for simulator

### Problem

Without proper CI integration, a broken PR could reach the review phase instead of being caught early, and without proper build tools, the Game design team has no easily accessible way to get a simulator build.
So, we need tooling and continuous integration for error checking code before it is sent to final review. This needs to be automatic and constant. We need automatic builds to enable this.

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

- Azure DevOps (may not be able to integrate it into github that easily)

##### Tooling

For setting up build environment:

- 'apt install' every run (simple but slower)

- Caching (a bit faster but more config)

- Docker image (fast and identical prebuilt environment, but requires maintenance)

For sharing builds with the Game Design team:

- Workflow artifacts (easy, but expires)

- Github releases (probably best for game design, but requires a bit more setup)

### Decision

Right now, the decision is pending because I would like Johann's input.

However, from the information I have gathered, it seems that Github Actions is the right choice here for CI.
This is because of the fact it is free, its close integration with Github, and being relatively low maintenance.

For builds, I am not as sure. Leaning towards apt install + caching at the moment.

### Open questions

- How should the chosen CI workflow be setup?

- What do people on the team specifically want? May have to change design depending on what

- Where should the workflows live?

- What PRs specifically trigger builds?
    - If PRs from the whole team trigger builds, is that within free limits?
    - How far can we go with free usage limits?

- Is a Linux simulator build enough for Game Design, or do we need Windows and Mac?

- What do other teams' builds need?
