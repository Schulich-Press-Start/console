# CI platform and build setup for simulator

### Problem

Without proper CI integration, a broken PR could reach the review phase instead of being caught early, and without proper build tools, the Game design team has no easily accessible way to get a simulator build.
So, we need tooling and continuous integration for error checking code before it is sent to final review. This needs to be automatic and constant. We need automatic builds to enable this.

**What we already have right now:**
- 32blit-sdk's cmake.yml already builds SDK and examples on every PR for Linux, Windows, Mac, STM32 and Pico at around 2.5min of runtime
- Builds stay downloadable for 90 days, and LIN64/WIN64 release zips can be attached to a GitHub release

**Whats missing:**
- No workflow currently builds or tests 32blit-linux yet, using four check template in onboarding guide (sync with aly so we dont end up with two)
- Nothing currently builds the buildroot image from external tree in console and publishes the cross-compiler sdk (currently, it likely runs on main/nightly instead of on every PR)

### Constraints

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

- **Github Actions** (interested in this one, easy and closely integrated with github)

- Azure DevOps (may not be able to integrate it into github that easily)

##### Tooling

For setting up build environment:

- 'apt install' every run (simple but slower)

- Caching (a bit faster but more config)

- Docker image (fast and identical prebuilt environment, but requires maintenance)

For sharing builds with the Game Design team:

- Windows MacOS and browser builds to share with Game Design (linux alone not enough)

- Workflow artifacts (easy, but expires)

- Github releases (probably best for game design, but requires a bit more setup)

### Decision

We have decided on using GitHub Actions as we are using public repositories. Also, Actions is already being used by console, 32blit-sdk and the website

For builds, we have decided on using multiple tools. We will use apt + caching (as the SDK already does this), only using Docker if the setup ends up getting slow. We will use artifacts for PRs, and releases for game design.

We will copy `timeout-minutes` from `console`'s build.yml as currently the sdk's cmake.yml does not set it

Game Design can use existing WIN64 and MacOS builds published in releases, with the browser build as a zero-install route

Workflows live in each repo's `.github/workflows` next to what they build.

However, I am still waiting on Johann's input on the build of the buildroot image

### Open questions

- What to do if MacOS runners stay unavailable?

- What do other teams' builds need?

- How often does the buildroot image currently (main or nightly?), and how do we cache it?

- Who fixes browser build's deploy step, and who owns the four-check workflow (Aly?)