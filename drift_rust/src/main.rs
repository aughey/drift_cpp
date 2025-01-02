use drift_rust::Parser;

fn main() -> anyhow::Result<()> {
    let hello_world_str = "Hello, World!";
    let hello_world_array = hello_world_str.as_bytes();

    // Stack allocated buffer.
    {
        let parsed = Parser::parse_owned(hello_world_array)?;
        assert_eq!(parsed.name, hello_world_str);
        let parsed = Parser::parse_zerocopy(hello_world_array)?;
        assert_eq!(parsed.name, hello_world_str);
    }

    // Heap allocated buffer.
    {
        let vec_buffer = hello_world_array.to_vec();

        let parsed = Parser::parse_owned(vec_buffer.as_ref())?;
        assert_eq!(parsed.name, hello_world_str);
        let parsed = Parser::parse_zerocopy(vec_buffer.as_ref())?;
        assert_eq!(parsed.name, hello_world_str);
    }

    // Heap allocated buffer free.
    {
        let parsed = {
            // vec_buffer is dropped after the block.
            let vec_buffer = hello_world_array.to_vec();
            Parser::parse_owned(vec_buffer.as_ref())?
        };
        assert_eq!(parsed.name, hello_world_str);

        #[cfg(feature = "uncompilable")]
        let parsed = {
            // vec_buffer is dropped after the block.
            let vec_buffer = hello_world_array.to_vec();
            Parser::parse(vec_buffer.as_ref())?
        };
        assert_eq!(parsed.name, hello_world_str);
    }

    // Heap allocated buffer changed
    {
        let mut vec_buffer = hello_world_array.to_vec();

        let parsed = Parser::parse_owned(vec_buffer.as_ref())?;
        // Change the original buffer.
        vec_buffer[0] = b'J';
        // We made a copy of the string, so it is not affected by the change.
        assert_eq!(parsed.name, hello_world_str);
        // Show that the original vec_buffer has been modified.
        assert_eq!(std::str::from_utf8(vec_buffer.as_ref())?, "Jello, World!");

        //reset vec_buffer
        vec_buffer = hello_world_array.to_vec();
        let parsed = Parser::parse_zerocopy(vec_buffer.as_ref())?;
        #[cfg(feature = "uncompilable")]
        {
            // Try to change the original buffer.
            vec_buffer[0] = b'J';
        }
        assert_eq!(parsed.name, hello_world_str);
    }

    Ok(())
}
