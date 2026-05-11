import wixData from 'wix-data';
import { authentication, currentMember } from 'wix-members-frontend';
import wixLocationFrontend from 'wix-location-frontend';
import wixWindowFrontend from 'wix-window-frontend';
import { triggeredEmails } from 'wix-crm';

// Constants
const COLLECTIONS = {
  posts: "Posts",
  subcontracting: "SubcontractingOpportunities"
};

const LIGHTBOXES = {
  error: "error",
  success: "success",
  createPost: "createPostLightbox"
};

const TIMEOUTS = {
  authentication: 10000,
  dataOperation: 15000
};

// Global variables
let currentUserId = null;
let currentTab = 0; // 0 for Posts, 1 for Subcontracting Opportunities
let isLoading = false;

// Utility Functions
function safeGetElement(selector) {
  try {
    const element = $w(selector);
    return element && element.length > 0 ? element : null;
  } catch (error) {
    console.error(`Element ${selector} not found:`, error);
    return null;
  }
}

function safeOpenLightbox(lightboxName, message) {
  try {
    return wixWindowFrontend.openLightbox(lightboxName, message);
  } catch (error) {
    console.error("Failed to open lightbox:", error);
    console.log("Message:", message);
    return Promise.resolve(null);
  }
}

function getAuthorName(authorId) {
  if (typeof authorId === 'object' && authorId && authorId.name) {
    return authorId.name;
  } else if (typeof authorId === 'string' && authorId.length > 0) {
    return "User";
  }
  return "Unknown";
}

function setLoadingState(isLoadingState) {
  isLoading = isLoadingState;
  const loadingElement = safeGetElement('#loading');
  if (loadingElement) {
    if (isLoadingState) {
      loadingElement.show();
    } else {
      loadingElement.hide();
    }
  }
}

function validatePostData(postData) {
  if (!postData) return false;
  if (!postData.content || postData.content.trim().length === 0) return false;
  if (!currentUserId) return false;
  return true;
}

// Main initialization
$w.onReady(async function () {
  setLoadingState(true);

  try {
    await initializeApp();
  } catch (error) {
    console.error("Critical initialization error:", error);
    safeOpenLightbox(LIGHTBOXES.error, 'Application failed to initialize. Please refresh the page.');
  } finally {
    setLoadingState(false);
  }
});

async function initializeApp() {
  try {
    // Check authentication with timeout
    const authPromise = authentication.loggedIn();
    const timeoutPromise = new Promise((_, reject) => 
      setTimeout(() => reject(new Error('Authentication timeout')), TIMEOUTS.authentication)
    );
    
    const isLoggedIn = await Promise.race([authPromise, timeoutPromise]);
    
    if (!isLoggedIn) {
      wixLocationFrontend.to('/contractor-signup?login=true');
      return;
    }

    // Get member data with validation
    const member = await currentMember.getMember();
    if (!member || !member._id) {
      console.error('Invalid member data received');
      wixLocationFrontend.to('/contractor-signup?login=true');
      return;
    }

    currentUserId = member._id;
    console.log('User initialized:', currentUserId);

    // Initialize page components
    initPage();
    setupSidebar();
    setupTabs();
    setupSearch();
    setupButtons();
    
    // Load initial data
    await loadPosts();

    // Show main content
    const postsSection = safeGetElement('#postsSection');
    if (postsSection) postsSection.show();
    
  } catch (error) {
    console.error("Error during app initialization:", error);
    throw error;
  }
}

function initPage() {
  try {
    // Hide all sections initially
    const sectionsToHide = ['#appsSection', '#receivedAppsSection', '#profileSection'];
    sectionsToHide.forEach(sectionId => {
      const section = safeGetElement(sectionId);
      if (section) section.hide();
    });

    // Show posts section by default
    const postsSection = safeGetElement('#postsSection');
    if (postsSection) postsSection.show();
    
  } catch (error) {
    console.error("Error initializing page:", error);
  }
}

function setupSidebar() {
  try {
    const sidebarConfig = [
      { button: '#postsBtn', section: '#postsSection' },
      { button: '#appsBtn', section: '#appsSection' },
      { button: '#receivedAppsBtn', section: '#receivedAppsSection' },
      { button: '#profileBtn', section: '#profileSection' }
    ];

    sidebarConfig.forEach(config => {
      const button = safeGetElement(config.button);
      if (button) {
        button.onClick(() => {
          // Hide all sections
          sidebarConfig.forEach(cfg => {
            const section = safeGetElement(cfg.section);
            if (section) section.hide();
          });
          
          // Show selected section
          const targetSection = safeGetElement(config.section);
          if (targetSection) targetSection.show();
          
          // Load posts if posts section is selected
          if (config.section === '#postsSection') {
            loadPosts();
          }
        });
      }
    });
  } catch (error) {
    console.error("Error setting up sidebar:", error);
  }
}

function setupTabs() {
  try {
    const tabsElement = safeGetElement('#communityTabs');
    if (tabsElement) {
      tabsElement.onChange((event) => {
        try {
          currentTab = event.target.selectedIndex || 0;
          loadPosts();
        } catch (error) {
          console.error("Error handling tab change:", error);
        }
      });
    }
  } catch (error) {
    console.error("Error setting up tabs:", error);
  }
}

function setupSearch() {
  try {
    const searchElement = safeGetElement('#searchInput');
    if (searchElement) {
      let searchTimeout;
      searchElement.onInput((event) => {
        try {
          // Debounce search
          clearTimeout(searchTimeout);
          searchTimeout = setTimeout(() => {
            const searchValue = event.target.value;
            if (!searchValue || searchValue.length > 2) {
              loadPosts();
            }
          }, 300);
        } catch (error) {
          console.error("Error handling search input:", error);
        }
      });
    }
  } catch (error) {
    console.error("Error setting up search:", error);
  }
}

function setupButtons() {
  try {
    setupQuickPostButton();
    setupCreatePostButton();
    setupPostRepeater();
  } catch (error) {
    console.error("Error setting up buttons:", error);
  }
}

function setupQuickPostButton() {
  const submitBtn = safeGetElement('#submitPostBtn');
  const quickPostInput = safeGetElement('#quickPostInput');
  
  if (submitBtn && quickPostInput) {
    submitBtn.onClick(async () => {
      try {
        if (isLoading) return;
        
        const content = quickPostInput.value?.trim();
        if (!content) {
          safeOpenLightbox(LIGHTBOXES.error, 'Please enter some content to post.');
          return;
        }

        if (content.length > 5000) {
          safeOpenLightbox(LIGHTBOXES.error, 'Post content is too long. Please keep it under 5000 characters.');
          return;
        }

        setLoadingState(true);
        
        const newPost = {
          title: "Quick Post",
          content: content,
          visibility: "Public",
          authorId: currentUserId,
          _createdDate: new Date(),
          type: currentTab === 0 ? "post" : "subcontracting"
        };

        if (!validatePostData(newPost)) {
          throw new Error('Invalid post data');
        }

        const collection = currentTab === 0 ? COLLECTIONS.posts : COLLECTIONS.subcontracting;
        await wixData.insert(collection, newPost);
        
        quickPostInput.value = "";
        await loadPosts();
        
        // Send notification email
        sendPostCreatedEmail(currentUserId, "Contractor");
        
        safeOpenLightbox(LIGHTBOXES.success, 'Quick post created successfully!');
        
      } catch (error) {
        console.error("Error creating quick post:", error);
        safeOpenLightbox(LIGHTBOXES.error, 'Error creating post: ' + error.message);
      } finally {
        setLoadingState(false);
      }
    });
  }
}

function setupCreatePostButton() {
  const createBtn = safeGetElement('#createNewPostBtn');
  if (createBtn) {
    createBtn.onClick(async () => {
      try {
        const result = await safeOpenLightbox(LIGHTBOXES.createPost, {
          title: "",
          content: "",
          visibility: "Public",
          authorId: currentUserId,
          type: currentTab === 0 ? "post" : "subcontracting"
        });
        
        if (result) {
          await loadPosts();
        }
      } catch (error) {
        console.error("Error opening create post lightbox:", error);
      }
    });
  }
}

function setupPostRepeater() {
  const repeater = safeGetElement('#postsRepeater');
  if (repeater) {
    repeater.onItemReady(($item, itemData) => {
      try {
        if (!itemData) return;

        // Set post content with safe defaults
        const titleElement = $item('#postTitle');
        const contentElement = $item('#postContent');
        const authorElement = $item('#postAuthor');
        const dateElement = $item('#postDate');

        if (titleElement) titleElement.text = itemData.title || "Untitled Post";
        if (contentElement) contentElement.text = itemData.content || itemData.description || "No content";
        if (authorElement) authorElement.text = getAuthorName(itemData.authorId);
        
        if (dateElement) {
          const date = itemData._createdDate ? new Date(itemData._createdDate) : new Date();
          dateElement.text = date.toLocaleDateString('en-US', { 
            year: 'numeric', 
            month: 'short', 
            day: 'numeric' 
          });
        }

        // Setup edit button
        const editBtn = $item('#postEditBtn');
        if (editBtn && itemData.authorId === currentUserId) {
          editBtn.show();
          editBtn.onClick(async () => {
            try {
              const result = await safeOpenLightbox(LIGHTBOXES.createPost, itemData);
              if (result) {
                await loadPosts();
              }
            } catch (error) {
              console.error("Error opening edit lightbox:", error);
            }
          });
        } else if (editBtn) {
          editBtn.hide();
        }

        // Setup delete button
        const deleteBtn = $item('#postDeleteBtn');
        if (deleteBtn && itemData.authorId === currentUserId) {
          deleteBtn.show();
          deleteBtn.onClick(async () => {
            try {
              if (isLoading) return;
              
              const confirmed = await safeOpenLightbox('confirmDelete', 'Are you sure you want to delete this post?');
              if (confirmed) {
                setLoadingState(true);
                const collection = currentTab === 0 ? COLLECTIONS.posts : COLLECTIONS.subcontracting;
                await wixData.remove(collection, itemData._id);
                await loadPosts();
                safeOpenLightbox(LIGHTBOXES.success, 'Post deleted successfully!');
              }
            } catch (error) {
              console.error("Error deleting post:", error);
              safeOpenLightbox(LIGHTBOXES.error, 'Error deleting post: ' + error.message);
            } finally {
              setLoadingState(false);
            }
          });
        } else if (deleteBtn) {
          deleteBtn.hide();
        }

      } catch (error) {
        console.error("Error setting up repeater item:", error);
      }
    });
  }
}

async function loadPosts() {
  if (isLoading) return;
  
  try {
    setLoadingState(true);
    
    if (!currentUserId) {
      throw new Error('User not authenticated');
    }

    const collection = currentTab === 0 ? COLLECTIONS.posts : COLLECTIONS.subcontracting;
    let query = wixData.query(collection)
      .eq("authorId", currentUserId)
      .limit(20)
      .descending("_createdDate");

    // Add search filter if search input exists and has value
    const searchElement = safeGetElement('#searchInput');
    const search = searchElement ? searchElement.value?.trim() : '';
    if (search && search.length > 2) {
      query = query.or(
        wixData.query(collection).contains("title", search),
        wixData.query(collection).contains("content", search)
      ).eq("authorId", currentUserId);
    }

    const results = await Promise.race([
      query.find(),
      new Promise((_, reject) => 
        setTimeout(() => reject(new Error('Query timeout')), TIMEOUTS.dataOperation)
      )
    ]);
    
    const repeaterElement = safeGetElement('#postsRepeater');
    if (repeaterElement) {
      const processedData = results.items.map(item => ({
        ...item,
        content: item.content || item.description || "No content",
        authorName: getAuthorName(item.authorId)
      }));
      
      repeaterElement.data = processedData;
      
      // Show no results message if needed
      const noResultsElement = safeGetElement('#noPostsMessage');
      if (noResultsElement) {
        if (processedData.length === 0) {
          noResultsElement.show();
        } else {
          noResultsElement.hide();
        }
      }
    }

  } catch (error) {
    console.error("Error loading posts:", error);
    safeOpenLightbox(LIGHTBOXES.error, 'Error loading posts: ' + error.message);
    
    // Clear repeater on error
    const repeaterElement = safeGetElement('#postsRepeater');
    if (repeaterElement) {
      repeaterElement.data = [];
    }
  } finally {
    setLoadingState(false);
  }
}

async function sendPostCreatedEmail(memberId, name) {
  try {
    if (!memberId || !name) {
      console.warn('Missing required email parameters');
      return;
    }

    await triggeredEmails.emailMember('Uj7TJWp', memberId, {
      variables: { 
        Name: name, 
        SITE_URL: wixLocationFrontend.baseUrl || "https://www.oneshoprepair.com" 
      }
    });
    
    console.log('Post creation email sent successfully');
  } catch (error) {
    console.error("Failed to send post created email:", error);
    // Don't show error to user for email failures
  }
}

// Export functions for potential lightbox use
export { loadPosts, currentUserId };