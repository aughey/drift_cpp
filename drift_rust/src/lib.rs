use anyhow::Result;

pub struct DataOwned {
    pub name: String,
}

pub struct ParserOwned {}
impl ParserOwned {
    pub fn parse(buffer: &[u8]) -> Result<DataOwned> {
        let name = String::from_utf8(buffer.to_vec())?;
        Ok(DataOwned { name })
    }
}

pub struct DataShared<'a> {
    pub name: &'a str,
}

pub struct ParserZeroCopy {}
impl ParserZeroCopy {
    pub fn parse(buffer: &[u8]) -> Result<DataShared<'_>> {
        let name = std::str::from_utf8(buffer)?;
        Ok(DataShared { name })
    }
}
