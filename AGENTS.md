# AGENTS.md - React Native Nitro Selectable Text

This document provides guidance for AI coding agents working in this repository.

## Project Overview

This is a **React Native Nitro Module** for creating a selectable text component. React Native's built-in `<Text/>` component handles text selection differently on iOS, so this module bridges native selectable text views using the [Nitro Modules](https://nitro.margelo.com) framework.

- **Framework**: Nitro Modules by mrousavy (Margelo)
- **iOS Implementation**: Swift
- **Android Implementation**: Kotlin
- **Component Type**: HybridView (native view component)

---

## Build & Development Commands

| Command              | Description                                                |
| -------------------- | ---------------------------------------------------------- |
| `bun run specs`      | Generate native bindings from `.nitro.ts` TypeScript specs |
| `bun run typecheck`  | Type-check TypeScript without emitting files               |
| `bun run lint`       | Lint and auto-fix with ESLint                              |
| `bun run typescript` | Compile TypeScript to `lib/` directory                     |
| `bun run clean`      | Remove build artifacts (`android/build`, `lib/`)           |

### Generating Native Code

After modifying any `*.nitro.ts` file in `src/specs/`, always regenerate native bindings:

```sh
bun run specs
```

This runs TypeScript compilation followed by `nitrogen --logLevel="debug"` to generate native interfaces in `nitrogen/generated/`.

### No Test Suite

This project does not currently have a test runner configured.

---

## Code Style Guidelines

### Prettier Configuration

- Single quotes (`'`)
- 2-space indentation (no tabs)
- ES5 trailing commas
- No semicolons
- Consistent quote props

### TypeScript

- **Strict mode** enabled with additional checks:
  - `noUncheckedIndexedAccess` - Array/object indexing returns `T | undefined`
  - `noUnusedLocals` / `noUnusedParameters` - No dead code
  - `noImplicitReturns` - All code paths must return
  - `verbatimModuleSyntax` - Must use `import type` for type-only imports

### Import Conventions

```typescript
// Use `import type` for type-only imports (enforced by verbatimModuleSyntax)
import { type HybridObject } from 'react-native-nitro-modules'

// Regular imports for values
import { getHostComponent } from 'react-native-nitro-modules'
```

### Naming Conventions

| Type                | Convention                      | Example                       |
| ------------------- | ------------------------------- | ----------------------------- |
| Interfaces/Types    | PascalCase                      | `SelectableText`, `TextRange` |
| HybridObject specs  | PascalCase + `.nitro.ts` suffix | `SelectableText.nitro.ts`     |
| Swift classes       | `Hybrid` prefix + `Spec` suffix | `HybridSelectableTextSpec`    |
| Kotlin classes      | `Hybrid` prefix + `Spec` suffix | `HybridSelectableTextSpec`    |
| Functions/variables | camelCase                       | `getText`, `selectedRange`    |
| Constants           | camelCase or UPPER_SNAKE        | `defaultValue`, `MAX_LENGTH`  |

---

## Project Structure

```
src/
  index.ts                 # Public exports (main entry point)
  specs/
    SelectableText.nitro.ts  # HybridObject TypeScript definitions

ios/
  Bridge.h                 # Bridging header (for Obj-C++ interop if needed)
  HybridSelectableText.swift  # Swift implementation (to be created)

android/
  src/main/java/com/margelo/nitro/nitroselectabletext/
    NitroSelectableTextPackage.kt  # React Native package registration
    HybridSelectableText.kt        # Kotlin implementation (to be created)
  src/main/cpp/
    cpp-adapter.cpp        # JNI initialization

nitrogen/
  generated/               # Auto-generated native bindings (COMMIT THIS)

nitro.json                 # Nitrogen configuration
NitroSelectableText.podspec  # iOS CocoaPods spec
```

---

## Nitro Module Development Workflow

### 1. Define TypeScript Spec

Create or modify specs in `src/specs/*.nitro.ts`:

```typescript
import { type HybridObject } from 'react-native-nitro-modules'

export interface SelectableText extends HybridObject<{
  ios: 'swift'
  android: 'kotlin'
}> {
  // Properties
  text: string
  selectedRange: { start: number; end: number } | undefined

  // Methods
  selectAll(): void
  clearSelection(): void
}
```

### 2. Generate Native Bindings

```sh
bun run specs
```

This generates `HybridSelectableTextSpec` protocol/class in `nitrogen/generated/`.

### 3. Implement in Swift (iOS)

Create `ios/HybridSelectableText.swift`:

```swift
class HybridSelectableText: HybridSelectableTextSpec {
  // Props
  var text: String = ""
  var selectedRange: TextRange? = nil

  // View (required for HybridView components)
  var view: UIView {
    return myTextView
  }

  // Methods
  func selectAll() throws {
    // Implementation
  }
}
```

### 4. Implement in Kotlin (Android)

Create `android/src/main/java/.../HybridSelectableText.kt`:

```kotlin
class HybridSelectableText(context: ThemedReactContext) : HybridSelectableTextSpec() {
  // Props
  override var text: String = ""
  override var selectedRange: TextRange? = null

  // View (required for HybridView components)
  override val view: View = TextView(context)

  // Methods
  override fun selectAll() {
    // Implementation
  }
}
```

### 5. Configure Autolinking

Update `nitro.json` to register the HybridObject:

```json
{
  "autolinking": {
    "SelectableText": {
      "swift": "HybridSelectableText",
      "kotlin": "HybridSelectableText"
    }
  }
}
```

### 6. Export from TypeScript

Update `src/index.ts`:

```typescript
import { getHostComponent } from 'react-native-nitro-modules'
import type {
  SelectableTextProps,
  SelectableTextMethods,
} from './specs/SelectableText.nitro'

export const SelectableText = getHostComponent<
  SelectableTextProps,
  SelectableTextMethods
>('SelectableText', () =>
  require('../nitrogen/generated/shared/json/SelectableTextConfig.json')
)
```

---

## Important Configuration Files

| File                          | Purpose                                                |
| ----------------------------- | ------------------------------------------------------ |
| `nitro.json`                  | Nitrogen config: namespaces, module names, autolinking |
| `NitroSelectableText.podspec` | iOS CocoaPods spec, includes nitrogen autolinking      |
| `android/build.gradle`        | Android Gradle config with CMake integration           |
| `android/CMakeLists.txt`      | C++ build config (C++20 standard)                      |
| `android/gradle.properties`   | SDK versions (minSdk=23, targetSdk=36, NDK=27.1)       |

---

## Native Implementation Notes

### iOS (Swift)

- Place Swift files in `ios/` directory
- The podspec automatically includes `ios/**/*.swift`
- Use `Bridge.h` for Objective-C++ bridging if needed
- Swift methods that can fail should use `throws`

### Android (Kotlin)

- Place Kotlin files in `android/src/main/java/com/margelo/nitro/nitroselectabletext/`
- Register ViewManagers in `NitroSelectableTextPackage.kt`
- Kotlin version: 2.1.20

### Cross-Platform C++ (Optional)

- Place C++ files in `cpp/` directory
- Add source files to `android/CMakeLists.txt`
- Add to podspec `source_files` for iOS

---

## Common Pitfalls

1. **Forgetting to regenerate specs** - Always run `bun run specs` after changing `.nitro.ts` files
2. **Not committing nitrogen/** - The `nitrogen/generated/` directory should be committed to git
3. **Type mismatches** - Ensure TypeScript types match Swift/Kotlin implementations exactly
4. **Missing autolinking** - Update `nitro.json` autolinking section for new HybridObjects

<skills_system priority="1">

## Available Skills

<!-- SKILLS_TABLE_START -->
<usage>
When users ask you to perform tasks, check if any of the available skills below can help complete the task more effectively. Skills provide specialized capabilities and domain knowledge.

How to use skills:
- Invoke: Bash("openskills read <skill-name>")
- The skill content will load with detailed instructions on how to complete the task
- Base directory provided in output for resolving bundled resources (references/, scripts/, assets/)

Usage notes:
- Only use skills listed in <available_skills> below
- Do not invoke a skill that is already loaded in your context
- Each skill invocation is stateless
</usage>

<available_skills>

<skill>
<name>brainstorming</name>
<description>"You MUST use this before any creative work - creating features, building components, adding functionality, or modifying behavior. Explores user intent, requirements and design before implementation."</description>
<location>global</location>
</skill>

<skill>
<name>code-reviewer</name>
<description>Comprehensive code review skill for TypeScript, JavaScript, Python, Swift, Kotlin, Go. Includes automated code analysis, best practice checking, security scanning, and review checklist generation. Use when reviewing pull requests, providing code feedback, identifying issues, or ensuring code quality standards.</description>
<location>global</location>
</skill>

<skill>
<name>dispatching-parallel-agents</name>
<description>Use when facing 2+ independent tasks that can be worked on without shared state or sequential dependencies</description>
<location>global</location>
</skill>

<skill>
<name>executing-plans</name>
<description>Use when you have a written implementation plan to execute in a separate session with review checkpoints</description>
<location>global</location>
</skill>

<skill>
<name>finishing-a-development-branch</name>
<description>Use when implementation is complete, all tests pass, and you need to decide how to integrate the work - guides completion of development work by presenting structured options for merge, PR, or cleanup</description>
<location>global</location>
</skill>

<skill>
<name>frontend-design</name>
<description>Create distinctive, production-grade frontend interfaces with high design quality. Use this skill when the user asks to build web components, pages, artifacts, posters, or applications (examples include websites, landing pages, dashboards, React components, HTML/CSS layouts, or when styling/beautifying any web UI). Generates creative, polished code and UI design that avoids generic AI aesthetics.</description>
<location>global</location>
</skill>

<skill>
<name>product-manager-toolkit</name>
<description>Comprehensive toolkit for product managers including RICE prioritization, customer interview analysis, PRD templates, discovery frameworks, and go-to-market strategies. Use for feature prioritization, user research synthesis, requirement documentation, and product strategy development.</description>
<location>global</location>
</skill>

<skill>
<name>receiving-code-review</name>
<description>Use when receiving code review feedback, before implementing suggestions, especially if feedback seems unclear or technically questionable - requires technical rigor and verification, not performative agreement or blind implementation</description>
<location>global</location>
</skill>

<skill>
<name>requesting-code-review</name>
<description>Use when completing tasks, implementing major features, or before merging to verify work meets requirements</description>
<location>global</location>
</skill>

<skill>
<name>senior-architect</name>
<description>Comprehensive software architecture skill for designing scalable, maintainable systems using ReactJS, NextJS, NodeJS, Express, React Native, Swift, Kotlin, Flutter, Postgres, GraphQL, Go, Python. Includes architecture diagram generation, system design patterns, tech stack decision frameworks, and dependency analysis. Use when designing system architecture, making technical decisions, creating architecture diagrams, evaluating trade-offs, or defining integration patterns.</description>
<location>global</location>
</skill>

<skill>
<name>subagent-driven-development</name>
<description>Use when executing implementation plans with independent tasks in the current session</description>
<location>global</location>
</skill>

<skill>
<name>systematic-debugging</name>
<description>Use when encountering any bug, test failure, or unexpected behavior, before proposing fixes</description>
<location>global</location>
</skill>

<skill>
<name>test-driven-development</name>
<description>Use when implementing any feature or bugfix, before writing implementation code</description>
<location>global</location>
</skill>

<skill>
<name>ui-design-system</name>
<description>UI design system toolkit for Senior UI Designer including design token generation, component documentation, responsive design calculations, and developer handoff tools. Use for creating design systems, maintaining visual consistency, and facilitating design-dev collaboration.</description>
<location>global</location>
</skill>

<skill>
<name>using-git-worktrees</name>
<description>Use when starting feature work that needs isolation from current workspace or before executing implementation plans - creates isolated git worktrees with smart directory selection and safety verification</description>
<location>global</location>
</skill>

<skill>
<name>using-superpowers</name>
<description>Use when starting any conversation - establishes how to find and use skills, requiring Skill tool invocation before ANY response including clarifying questions</description>
<location>global</location>
</skill>

<skill>
<name>ux-researcher-designer</name>
<description>UX research and design toolkit for Senior UX Designer/Researcher including data-driven persona generation, journey mapping, usability testing frameworks, and research synthesis. Use for user research, persona creation, journey mapping, and design validation.</description>
<location>global</location>
</skill>

<skill>
<name>verification-before-completion</name>
<description>Use when about to claim work is complete, fixed, or passing, before committing or creating PRs - requires running verification commands and confirming output before making any success claims; evidence before assertions always</description>
<location>global</location>
</skill>

<skill>
<name>writing-plans</name>
<description>Use when you have a spec or requirements for a multi-step task, before touching code</description>
<location>global</location>
</skill>

<skill>
<name>writing-skills</name>
<description>Use when creating new skills, editing existing skills, or verifying skills work before deployment</description>
<location>global</location>
</skill>

</available_skills>
<!-- SKILLS_TABLE_END -->

</skills_system>
