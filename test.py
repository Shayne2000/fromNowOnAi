import kagglehub

# Download latest version
path = kagglehub.dataset_download("azeembootwala/titanic")

print("Path to dataset files:", path)