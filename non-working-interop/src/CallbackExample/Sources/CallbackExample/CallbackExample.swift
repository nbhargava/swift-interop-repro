import AppKit

public typealias Callback = @convention(c) (UnsafePointer<CChar>?) -> Void

@_cdecl("register_for_changes")
public func registerForChanges(_ callback: @escaping Callback) {
    let arg = "arg"
    arg.withCString { (cstr) in
        callback(cstr)
    }
}

@_cdecl("swift_fibonacci")
public func fibonacci(_ n: CInt) -> CInt {
    if n == 0 {
        return 0
    } else if n == 1{
        return 1
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}
