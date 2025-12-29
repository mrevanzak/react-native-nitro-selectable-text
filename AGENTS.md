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
